#!/usr/bin/python
# -*- coding: utf-8 -*-

import os, sys, socket, time, optparse, json, struct


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


def udp_send(host, port, timeout, fb, verbose):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    dump = "".join(["%02X" % i for i
                    in struct.unpack('>%dB' % (len(fb)), fb)])
    data = list()
    dump_bytes_to_send = len(dump)
    sz = 64  # chunk size
    sb = 0   # chunk start byte
    while sb < dump_bytes_to_send:
        st = dump_bytes_to_send - sb
        st = (st, sz)[st > sz]
        data.append(dump[sb:sb+st])
        sb = sb + st

    for n, i in enumerate(data):
        print "%010X: %s %s %s %s  %s %s %s %s" %\
              (n*sz/2,
               i[  : 8], i[ 8:16], i[16:24], i[24:32],
               i[32:40], i[40:48], i[48:56], i[56:64])

    total_bytes = 0
    send_start_time = time.clock()
    minimal_system_timeout = 0.009  # tested only on win32 XP SP2 i386
    timeout_sum = 0.0
    no_sleep_sends = 0
    for n, i in enumerate(data):
        buff = "%d %s" % (n, i)
        total_bytes += len(buff)+1
        sock.sendto(buff, (host, port))
        timeout_sum += timeout
        no_sleep_sends += 1
        if timeout_sum > minimal_system_timeout:
            sleep_begin_time = time.clock()
            time.sleep(timeout_sum)
            sleep_time = time.clock() - sleep_begin_time
            if verbose:
                print "try sleep %.2f ms after %d sends" %\
                      ((1000 * timeout_sum), no_sleep_sends)
                print "can sleep %.2f ms (timeout %.2f ms)" %\
                      ((1000 * sleep_time), (1000 * timeout))
            timeout_sum = timeout_sum - sleep_time
            no_sleep_sends = 0

    total_send_time = time.clock() - send_start_time

    print "File send time:", total_send_time, "seconds"
    if total_send_time > 0:
        print "Send", total_bytes, "bytes in", len(data), "datagrams"
        print float(total_bytes)/(128*1024*total_send_time), "Mbits per second"
        print float(len(data))/total_send_time, "Datagrams per second"


def main():
    parser = optparse.OptionParser()
    parser.add_option(
        "-v", "--verbose",
        action="store_true", dest="verbose", default=False,
        help="Print additional send info to stdout")
    parser.add_option(
        "-i", "--ip", dest="ip", metavar="127.0.0.1",
        default="127.0.0.1",
        help="Target host ipv4, default is localhost")  # optional
    parser.add_option(
        "-p", "--port", dest="port", metavar='PORT',
        help="Target UDP port")
    parser.add_option(
        "-f", "--file", dest="file", metavar='"../my/file.js"',
        help="File path")
    parser.add_option(
        "-t", "--timeout", dest="timeout", metavar='TIMEOUT',
        default=10,
        help="Sleep between datagrams. Default 10 ms")  # optional
    options, args = parser.parse_args()

    if not options.port:
        sys.stderr.write("ERROR: destination UDP port not defined\n")
        sys.stdout.write(parser.format_help())
        sys.exit(1)

    if not options.file:
        sys.stderr.write("ERROR: source file path not defined\n")
        sys.stdout.write(parser.format_help())
        sys.exit(1)

    host, port = options.ip, int(options.port)
    path = os.path.abspath(os.path.normpath(options.file))
    timeout = float(options.timeout) / 1000.0

    if timeout < 0.0001:  # minimal timeout (tested on win32 nt i3)
        timeout = 0.0001

    if not os.path.exists(path):
        sys.stderr.write("ERROR: file not exists %s\n" % path)
        sys.stdout.write(parser.format_help())
        sys.exit(1)

    if not os.path.isfile(path):
        sys.stderr.write("ERROR: is not file %s\n" % path)
        sys.stdout.write(parser.format_help())
        sys.exit(1)

    print 'Read file at path \"%s\"' % path
    print 'Send to UDP %s:%d with timeout %s seconds' % (host, port, timeout)

    read_start_time = time.clock()
    f = file(path, "rb");
    buff = f.read()
    f.close()
    total_read_time = time.clock() - read_start_time

    print "File read time:", total_read_time, "seconds"
    udp_send(host, port, timeout, buff, verbose=options.verbose)


if __name__ == '__main__':
    # options, args = parser.parse_args()
    main()
