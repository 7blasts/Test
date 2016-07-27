#!/usr/bin/python
import socket
import time

IP = "127.0.0.1"
PORT = 45455

print "UDP target %s:%d" % (IP, PORT)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
total_bytes = 0
start_time = time.clock()
for i in xrange(4096):
    buff = "%X:%s" % (i, 86*str(time.clock()))
    total_bytes += len(buff)+1
    sock.sendto(buff, (IP, PORT))
    time.sleep(0.001)  # process must sleep between UDP-send datagram to
    #                  # prevent data overwriting in target UDP buffer

total_send_time = time.clock() - start_time

print total_send_time, "seconds"
if total_send_time > 0:
    print "Send", total_bytes, "bytes"
    print float(total_bytes)/(128*1024*total_send_time), "Mbits per second"
    print float(4096)/total_send_time, "Datagrams per second"
