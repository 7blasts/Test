#!/usr/bin/python
# -*- coding: utf-8 -*-

import os, sys, socket, time, optparse, struct


class Unbuffered(object):  # make sys.stdout and sys.stderr unbuffered
    def __init__(self, stream):
        self.stream = stream

    def write(self, data):
        self.stream.write(data)
        self.stream.flush()

    def __getattr__(self, attr):
        return getattr(self.stream, attr)

sys.stdout = Unbuffered(sys.stdout)
sys.stderr = Unbuffered(sys.stderr)


def encode_hex_dump(buff, verbose=False):
    encode_time_start = time.clock()
    dump = "".join(
        ["%02X" % i for i in struct.unpack('>%dB' % (len(buff)), buff)]
    )
    encode_time = time.clock() - encode_time_start
    if verbose:
        file_size = len(buff)
        dump_size = len(dump)
        sys.stdout.write("File size %s\n" % file_size)
        sys.stdout.write("Dump size %s\n" % dump_size)
        sys.stdout.write("Encode time %s\n" % encode_time)
    return dump


def encode_bytearray(buff, verbose=False):
    encode_time_start = time.clock()
    dump = bytearray(buff)
    encode_time = time.clock() - encode_time_start
    if verbose:
        file_size = len(buff)
        dump_size = len(dump)
        sys.stdout.write("File size %s\n" % file_size)
        sys.stdout.write("Dump size %s\n" % dump_size)
        sys.stdout.write("Encode time %s\n" % encode_time)
    return dump


def udp_send(host, port, timeout, dump, step, verbose):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    data = list()
    dump_bytes_to_send = len(dump)
    sz = step-8  # is step < 512, so <chunk-number-8>:<data-buff-504>
    sb = 0    # chunk start byte
    while sb < dump_bytes_to_send:
        st = dump_bytes_to_send - sb
        st = (st, sz)[st > sz]
        data.append(dump[sb:sb+st])
        sb = sb + st

    if verbose > 1:
        for n, i in enumerate(data):
            lb = 0                       # left bound
            cl = 0                       # dump column number
            columns_per_stdout_line = 8
            sys.stdout.write("%08X:" % n)
            while lb < len(i):
                ls = len(i) - lb
                ls = (ls, 8)[ls > 8]  # grid step
                sys.stdout.write(" %s" % i[lb: lb+ls])
                lb += ls
                cl += 1
                if cl > columns_per_stdout_line:
                    sys.stdout.write("\n         ")
                    cl = 0
            sys.stdout.write("\n")

    total_bytes = 0
    send_start_time = time.clock()
    minimal_system_timeout = 0.009  # tested only on win32 XP SP2 i386
    timeout_sum = 0.0
    no_sleep_sends = 0
    for n, i in enumerate(data):
        buff = "%08X%s" % (n, i)
        total_bytes += len(buff)+1
        sock.sendto(buff, (host, port))
        timeout_sum += timeout
        no_sleep_sends += 1
        if timeout_sum > minimal_system_timeout:
            sleep_begin_time = time.clock()
            time.sleep(timeout_sum)
            sleep_time = time.clock() - sleep_begin_time
            if verbose > 2:
                sys.stdout.write(
                    "try sleep %.2f ms after %d sends\n" % (
                        (1000 * timeout_sum), no_sleep_sends
                    )
                )
                sys.stdout.write(
                    "can sleep %.2f ms (timeout %.2f ms)\n" % (
                        (1000 * sleep_time), (1000 * timeout)
                    )
                )
            timeout_sum = timeout_sum - sleep_time
            no_sleep_sends = 0

    total_send_time = time.clock() - send_start_time

    if verbose:
        print "File send time:", total_send_time, "seconds"
        if total_send_time > 0:
            sys.stdout.write(
                "Send %s bytes " % total_bytes)
            sys.stdout.write(
                "in %s datagrams\n" % len(data))
            sys.stdout.write(
                "Last datagram ID 0x%011X\n" % (len(data)-1))
            sys.stdout.write(
                "%f Mbits per second\n" % (
                    float(total_bytes)/(128*1024*total_send_time)))
            sys.stdout.write(
                "%f Datagrams per second\n" % (
                    float(len(data))/total_send_time))


def main():
    parser = optparse.OptionParser()

    parser.add_option(
        "-v", "--verbose",  # action="store_true",
        dest="verbose", type='int', default=1,
        help="Print additional send info to stdout")

    parser.add_option(
        "-i", "--ip", dest="ip", default="127.0.0.1",
        metavar="127.0.0.1",
        help="Target host ipv4, default is localhost")  # optional

    parser.add_option(
        "-p", "--port", dest="port", type='int',  metavar='PORT',
        help="Target UDP port")

    parser.add_option(
        "-f", "--file", dest="file", metavar='"../my/file.js"',
        help="File path")

    parser.add_option(
        "-t", "--timeout", dest="timeout", type='float', default=10,
        metavar='TIMEOUT',
        help="Sleep between datagrams. Default 10 ms")  # optional

    parser.add_option(
        "-s", "--size", dest="datagram_size", default=512, type='int',
        metavar='DATAGRAM_SIZE',
        help="Datagram size. Default 512 byte")  # optional

    parser.add_option(
        "-c", "--codec", dest="codec", default="hex_dump",
        metavar='codec_name',
        help="Codec name. Used 'hex_dump' by default")  # optional

    options, args = parser.parse_args()

    if not options.port:
        sys.stderr.write("ERROR: destination UDP port not defined\n")
        sys.exit(1)

    if not options.file:
        sys.stderr.write("ERROR: source file path not defined\n")
        sys.exit(1)

    if not hasattr(sys.modules["__main__"], "encode_" + options.codec):
        sys.stderr.write("ERROR: undefined codec '%s'" % options.codec)
        sys.stderr.write("\n\t-c%s\n" % options.codec)
        sys.exit(1)

    path = os.path.abspath(os.path.normpath(options.file))

    if not os.path.exists(path):
        sys.stderr.write("ERROR: file not exists %s\n" % path)
        sys.stderr.write("\n\t-f%s\n" % options.file)
        sys.exit(1)

    if not os.path.isfile(path):
        sys.stderr.write("ERROR: is not file %s\n" % path)
        sys.stderr.write("\n\t-f%s\n" % options.file)
        sys.exit(1)

    host, port = options.ip, int(options.port)
    timeout = float(options.timeout) / 1000.0
    timeout = (timeout, 0.0001)[timeout < 0.0001]

    if options.datagram_size < 9:
        sys.stderr.write("WARNING: datagram size is too small %s\n"
                         % options.datagram_size)
        sys.stderr.write("\t-s%d changed to 9\n" % options.datagram_size)
        options.datagram_size = 9

    read_start_time = time.clock()
    f = file(path, "rb")
    buff = f.read()
    f.close()

    if options.verbose:
        total_read_time = time.clock() - read_start_time
        sys.stdout.write('Read file at path \"%s\"\n' % path)
        sys.stdout.write('Send UDP %s : %d, timeout %s seconds\n'
                         % (host, port, timeout))
        sys.stdout.write('Send datagram size %d\n' % options.datagram_size)
        sys.stdout.write("File read time %f seconds\n" % total_read_time)
    buff = encode_hex_dump(buff,
                           verbose=options.verbose)
    udp_send(host, port, timeout,
             buff, options.datagram_size,
             verbose=options.verbose)


def self_test_case():
    sys.argv += ["-f", "C:/windows/system32/notepad.exe"]
    sys.argv += ["-p", "45455"]       # -p45455
    sys.argv += ["-t", "10"]        # -t1.33
    sys.argv += ["-s1024", ]          # default is 512
    # sys.argv += ["-c", "bytearray"]   # default is -c hex_dump
    sys.argv += ["-v", "1"]         # [0|1|2] default is 1
    main()

if __name__ == '__main__':
    if len(sys.argv) > 1:
        main()
    else:
        self_test_case()
