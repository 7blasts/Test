#!/usr/bin/python
# -*- coding: utf-8 -*-
#
#  This script, like our "udp_send.py" also send file in UDP socket, as some
# set of datagrams, but after send each -t "no_sleep" datagrams wait response
# from target socket. If target socket are not responding, interrupt process.
#
# Usage:
#  -h, --help            Print this usage info
#  -v INT, --verbose=INT How match info print in stdout 0|1|2. Default: -v1
#  -i HOST, --ip=HOST    Target host ipv4, default is localhost: -i127.0.0.1
#  -p PORT, --port=PORT  Target UDP port, example -p45455
#  -f PATH, --file=PATH  File path, example -f"C:\WINDOW\system32\notepad.exe"
#  -s INT, --size=INT    Datagram size. Default: -s512
#  -t INT, --times=INT   Times to send before wait response. Default: -t 5
#  -c STR, --codec=STR   Codec names [dump|b64|hex]. Default: -c dump
#  WARNING: Start w/o command line arguments switch process to self test mode
#
import os, sys, socket, time, optparse, struct, json


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

cout = sys.stdout.write
cerr = sys.stderr.write


def encode_hex(buff, verbose=False):
    encode_time_start = time.clock()
    dump = "".join(
        ["%02X" % i for i in struct.unpack('>%dB' % (len(buff)), buff)]
    )
    encode_time = time.clock() - encode_time_start
    if verbose:
        file_size = len(buff)
        dump_size = len(dump)
        cout("File size %s\n" % file_size)
        cout("Dump size %s\n" % dump_size)
        cout("Encode time %s\n" % encode_time)
    return dump


def encode_dump(buff):
    return bytearray(buff)


def print_datagram(head, body, columns=8):
    lh = len(head)
    lb = 0  # curr left bound
    cl = 0  # curr column number
    co = (columns - 1, 0)[columns < 1]
    cout(head)
    while lb < len(body):
        ls = len(body) - lb
        ls = (ls, 8)[ls > 8]  # grid step
        cout(" %s" % body[lb: lb+ls])
        lb += ls
        cl += 1
        if cl > co:
            cout("\n" + lh * " ")
            cl = 0
    cout("\n")


def udp_wait_response(sock):
    try:
        buff = sock.recv(4096)
        return '%s' % buff
    except socket.timeout:
        pass
    except socket.error:
        pass
    return ""


def udp_query(sock, host, port, verbose, text):
    query_response_begin_time = time.clock()
    query_counter, query_counter_max = 0, 15
    for query_counter in xrange(query_counter_max):
        sock.sendto(text, (host, port))
        response = udp_wait_response(sock)
        if response:
            break
    if verbose > 2:
        query_response_time = time.clock() - query_response_begin_time
        cout("%s:%d<%s> try %02d time %f:%s\n" % (
            host, port, text, query_counter+1, query_response_time, response
        ))
    return response if response else ""


def udp_send(sock, host, port, data, head, times, verbose):
    data = [head, ] + data
    has_no_response = False
    total_bytes = 0
    send_start_time = time.clock()
    need_response = []
    #  response = ""
    for n, i in enumerate(data):
        head = "%08X" % n
        buff = head + i
        if verbose > 2:
            print_datagram(head, i, columns=8)
        total_bytes += len(buff)+1
        sock.sendto(buff, (host, port))
        need_response.append(n)
        if len(need_response) >= times:
            response = udp_query(
                sock, host, port, verbose,
                json.dumps(["echo", n, len(data), need_response])
            )
            if response:
                try:
                    echo = json.loads(response)
                except Exception, e:
                    cout("     <invalid><%s> \n" % type(e))
                    break
                if verbose > 2:
                    cout("     <success>%s\n" % echo)
                need_response = []
            else:
                has_no_response = True
                break

    if not has_no_response:
        final_response = udp_query(
            sock, host, port, verbose,
            '["exit", %d, %d ]' % (n+1, len(data))
        )
        if final_response and verbose:
            cout("Final message is %s\n" % final_response)

    total_send_time = time.clock() - send_start_time

    if verbose:
        cout("File send time: %f seconds\n" % total_send_time)
        if total_send_time > 0:
            cout("Send %s bytes " % total_bytes)
            cout("in %s datagrams\n" % len(data))
            cout("Last datagram ID 0x%011X\n" % (len(data)-1))
            cout("%f Mbits per second\n" % (
                float(total_bytes)/(128*1024*total_send_time)))
            cout("%f Sends per second\n" % (
                float(len(data))/total_send_time))
            cout(("Ok.\n", "FAILED!\n")[has_no_response])


def udp_send_data(sock, host, port, data, head, times, verbose):
    send_start_time = time.clock()
    # Before send file body we need response from target. Some part
    # of this file, in current encoding, can be exists on the target.
    # Target must return start position and parts to send before wait
    # next response. All 'data' items from "0" to "start_position" we
    # can set to None
    response = udp_query(sock, host, port, verbose, head)
    while response:  # if no response go to unexpected "connection lost"
        try:
            echo = json.loads(response)
        except ValueError:
            cout("ERROR: host echo is not JSON dump\n")
            cout('        <%s>\n' % echo)
            return
        if type(echo) != dict:
            cout("ERROR: host echo is not 'dict'")
            return
        if "parts" in echo:
            on_air = echo["parts"]
            if type(on_air) != list:
                cout('FAILED: "main:parts" must be list of int\n')
                cout('        <%s>\n' % echo)
                return
            for n in on_air:
                sock.sendto(data[n], (host, port))
            response = udp_query(
                sock, host, port, verbose,
                json.dumps(on_air)
            )
            continue

        if "exit" in echo:
            cout('SUCCESS: host send "exit"\n')
            return

        if "wait" in echo:
            wait_ms, wait_id = host_response["timeout"]
            time.sleep(float(wait_ms)/1000.0)
            response = udp_query(sock, host, port, verbose, json.dumps(dict(
                wait_responce=[wait_ms, wait_id]
            )))
            continue

        if "start_pos" in echo:
            start_pos = echo["start_pos"] if "start_pos" in echo else 0
            if type(start_pos) != int:
                cout('FAILED: "init:start_pos" must be "int":\n')
                cout('        <%s>\n' % echo)
                return

            step = echo["step"] if "step" in echo else 1
            if type(start_pos) != int:
                cout('FAILED: "init:step" must be "int":\n')
                cout('        <%s>\n' % echo)
                return

            if verbose == verbose:
                cout('QUERY_START_POS:%s\n' % echo)

            tail_parts = len(data) - start_pos
            step_parts = step if tail_parts > step else tail_parts
            on_air = [start_pos+j for j in xrange(step_parts)]
            for n in on_air:
                sock.sendto(data[n], (host, port))
            response = udp_query(
                sock, host, port, verbose,
                json.dumps(on_air)
            )
            continue
        cout("WARNING: unexpected exit (unexpected response type)\n")
        return
    cout("WARNING: unexpected exit (host no response)\n")


def main():
    parser = optparse.OptionParser()

    parser.add_option(
        "-v", "--verb",  metavar="INT",  # action="store_true",
        dest="verbose", type='int', default=1,
        help="log level 0-silent, 1-minimal, 2-max, default -v1")
    parser.add_option(
        "-i", "--ip", dest="ip", default="127.0.0.1",
        metavar="HOST",
        help="target host ipv4, default -i127.0.0.1")
    parser.add_option(
        "-p", "--port", dest="port", type='int',  metavar="PORT",
        help="target UDP port")
    parser.add_option(
        "-f", "--file", dest="file", metavar="PATH",
        help="where is file to send")
    parser.add_option(
        "-t", "--times", dest="times", type='int', default=3,
        metavar='INT', help="times send before wait, default -t 3")
    parser.add_option(
        "-s", "--size", dest="datagram_size",  type='int', default=512,
        metavar='INT', help="datagram size, default -s 512")
    parser.add_option(
        "-c", "--codec", dest="codec", default="hex", metavar='NAME',
        help="codec name: 'b64' or 'hex' or 'dump', default -c hex")
    options, args = parser.parse_args()

    if not options.port:
        cerr("ERROR: destination UDP port not defined\n")
        sys.exit(1)

    if not options.file:
        cerr("ERROR: source file path not defined\n")
        sys.exit(1)

    if not hasattr(sys.modules["__main__"], "encode_" + options.codec):
        cerr("ERROR: undefined codec '%s'" % options.codec)
        cerr("\n\t-c%s\n" % options.codec)
        sys.exit(1)

    path = os.path.abspath(os.path.normpath(options.file))
    if not os.path.exists(path):
        cerr("ERROR: file not exists %s\n" % path)
        cerr("\n\t-f%s\n" % options.file)
        sys.exit(1)

    if not os.path.isfile(path):
        cerr("ERROR: is not file %s\n" % path)
        cerr("\t-f%s\n" % options.file)
        sys.exit(1)

    if options.times < 1:
        cerr("WARNING: sends before wait is too small %d\n" % options.times)
        cerr("\t-t%d changed to 1\n" % options.times)
        options.times = 1

    if options.datagram_size < 9:
        cerr("WARNING: datagram size too small %s\n" % options.datagram_size)
        cerr("\t-s%d changed to 9\n" % options.datagram_size)
        options.datagram_size = 9

    host, port = options.ip, int(options.port)
    read_start_time = time.clock()
    f = file(path, "rb")
    buff = f.read()
    file_size = len(buff)
    f.close()

    if options.verbose:
        total_read_time = time.clock() - read_start_time
        cout("Read file at path \"%s\"\n" % path)
        cout("Send UDP %s : %d, times send before response %d\n" % (
            host, port, options.times
        ))
        cout("Send datagram size %d\n" % options.datagram_size)
        cout("File read time %f seconds\n" % total_read_time)

    encode = getattr(sys.modules["__main__"], "encode_" + options.codec)
    encode_time_start = time.clock()
    buff = encode(buff)

    data = list()
    dump_bytes_to_send = len(buff)
    sz = options.datagram_size - 8  # here len(each_datagram_head)==8 bytes
    cn = 0   # part's number
    sb = 0   # chunk start byte
    while sb < dump_bytes_to_send:
        st = dump_bytes_to_send - sb
        st = (st, sz)[st > sz]
        data.append("%08X" % cn + buff[sb:sb+st])
        sb = sb + st
        cn += 1

    # Протокол UDP передатчика файла. Запрашивает протокол UDP приёмника файла.
    #
    # Многократное послание "Запрос Интерфейса" с частотой get-time-out-сокета.
    # Приёмник может получить ещё несколько таких, даже если уже принял сессию
    # и успел отправить ответы и запросы в рамках запрошенного интерфейса:
    head = json.dumps(
        dict(                             # ==> JSON:DICT:QueryInterface: ????
            mtype="FtpReceiver",          # Query interface ftp receiver  str+
            query=options.file,           # Send back queried file path   str+
            codec=options.codec,          # Encode method                 str+
            parts=len(data),              # Parts count                   int+
            psize=options.datagram_size,  # Each part size                int+
            tsize=len(data[-1]),          # Tail part size                int+
            stime=time.clock()            # Session-start-time-stamp    float+
        ), separators=(',', ':')          # <== JSON:DICT:FtpReceiver
    )

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.settimeout(0.2)

    if options.verbose:
        encode_time = time.clock() - encode_time_start
        dump_size = len(buff)
        cout("Codec name \"%s\"\n" % options.codec)
        cout("File size %s\n" % file_size)
        cout("Dump size %s\n" % dump_size)
        cout("Encode time %s\n" % encode_time)

    udp_send_data(
        sock, host, port, data,   # host, port, list of encoded-data-chunks
        head,
        options.times,            # times send before wait response
        options.verbose           # 0-silent, 1-minimal log, 2-maximum info
    )


def self_test_case():
    #sys.argv += ["-f", "C:/windows/system32/notepad.exe"]
    sys.argv += ["-f", "howto_udp_send.py"]
    sys.argv += ["-p45455", ]    # -p45455
    sys.argv += ["-t5", ]        # "sends before query response" default -t3
    sys.argv += ["-s512", ]      # default is -s 512
    #sys.argv += ["-c", "dump"]  # default is -c hex
    sys.argv += ["-v", "3"]      # [0|1|2] default is 1
    main()

if __name__ == '__main__':
    if len(sys.argv) > 1:
        main()
    else:
        self_test_case()
