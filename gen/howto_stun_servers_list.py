# https://gist.github.com/zziuni/3741933
import sys
import time
import socket
import struct
import random

cout = sys.stdout.write
verbose_servers_dict = dict()
stun_servers_candidates = {
    "stun.l.google.com"        : dict(port=19305),  # 19302
    "stun1.l.google.com"       : dict(port=19305),  # 19302
    "stun2.l.google.com"       : dict(port=19305),  # 19302
    "stun3.l.google.com"       : dict(port=19305),  # 19302
    "stun4.l.google.com"       : dict(port=19305),  # 19302
    "stun.ekiga.net"           : dict(),
    "stun.ideasip.com"         : dict(),
    "stun.schlund.de"          : dict(),
    "stun.voiparound.com"      : dict(),
    "stun.voipbuster.com"      : dict(),
    "stun.voipstunt.com"       : dict(),
    "stun.voxgratia.org"       : dict(),
    "numb.viagenie.ca"         : dict(),
    "stun.counterpath.com"     : dict(),
    "stun.counterpath.net"     : dict(),
    "stun.noc.ams-ix.net"      : dict(),
    "stun.internetcalls.com"   : dict(),
    "stun.voip.aebc.com"       : dict(),
    "stun.callwithus.com"      : dict(),
    "stun.services.mozilla.com": dict(),
}

stun_servers_candidates_unstable = {
    "stun.rixtelecom.se": {},
    "stunserver.org": {},
    "stun.softjoys.com": {},
    "stun.xten.net": {},
    "sip.iptel.org": {},
    "stun2.wirlab.net": {},
    "stun.wirlab.net": {},
    "stun1.vovida.org": {},
    "stun.xten.com": {},
    "s1.taraba.net": {},
    "s2.taraba.net": {},
    "s1.voipstation.jp": {},
    "s2.voipstation.jp": {},
    "stun.iptel.org": {},
    "stun01.sipphone.com": {},
    "tesla.divmod.net": {},
    "erlang.divmod.net": {},
    "stun.fwdnet.net": {},
    "stun2.fwdnet.net": {},
    "stun.fednet.net": {},
    "stun2.fednet.net": {},
    "stun.stunprotocol.prg": {},
    "provserver.televolution.net": dict(),  #  (out of service)
    "sip1.lakedestiny.cordiaip.com": dict(),  #  (out of service)
    "stun1.voiceeclipse.net": dict(),  #
    "stun.endigovoip.com": dict(),  #  (out of service)
    "stun.ipns.com": dict(),           #  (out of service)
    "stun.phonepower.com": dict(),     # (out of service)
    "stun.phoneserve.com": dict(),     #
    "stun.rnktel.com": dict(),         # (out of service)
    "stun.sipgate.net": dict(),
    "stun.stunprotocol.org": dict(),
    "stun.voxalot.com": dict(),    #(out of service)
    # UA-IX zone http://www.ipshka.com/main/help/hlp_stun.php
    "stun.ipshka.com": dict(),
}

StunTypes = {
    b'\x00\x00': '(reserved)',
    b'\x00\x01': 'MAPPED-ADDRESS',
    b'\x00\x02': 'RESPONSE-ADDRESS ',
    b'\x00\x03': 'CHANGE-REQUEST',
    b'\x00\x04': 'SOURCE-ADDRESS',
    b'\x00\x05': 'CHANGED-ADDRESS',
    b'\x00\x06': 'USERNAME',
    b'\x00\x07': 'PASSWORD',
    b'\x00\x08': 'MESSAGE-INTEGRITY',
    b'\x00\x09': 'ERROR-CODE',
    b'\x00\x0a': 'UNKNOWN-ATTRIBUTES',
    b'\x00\x0b': 'REFLECTED-FROM',
    b'\x00\x14': 'REALM',
    b'\x00\x15': 'NONCE',
    b'\x00\x20': 'XOR-MAPPED-ADDRESS',
    b'\x80\x20': 'XOR-MAPPED-ADDRESS',  # ? are you really
    b'\x80\x22': 'SOFTWARE',
    b'\x80\x23': 'ALTERNATE-SERVER',
    b'\x80\x28': 'FINGERPRINT',
}


def get_random_bytes(sz):
    random.seed(time.clock())
    return [random.randint(0, 255) for _ in range(sz)]


def extract_ip(binary):
    return '.'.join([str(j) for j in struct.unpack('BBBB', binary)])


def hex_ip(str_ip):
    """Convert str() ip to str hex dump
    :param str_ip:           example str('127.0.0.1')
    :return: str() hex dump: example str('7F000001')
    """
    return "".join(["%02X" % int(j) for j in str_ip.split('.')])


def hex_port(unsigned_short_port):
    return "%04X" % int(unsigned_short_port)


def hex_dump(bytearray_data):
    return "".join(["%02X" % int(b) for b in bytearray_data])


def hex_port(unsigned_short_port):
    return "%04X" % int(unsigned_short_port)


def hex_hp(host_port_tuple):
    """Convert host:port tuple to str hex dump. Example
     tuple('127.0.0.1', 45455) => str('7F000001B18F')
    :param host_port_tuple: example ('127.0.0.1', 45455)
    :return: return str() hex dump: example '7F000001B18F'
    """
    return hex_ip(host_port_tuple[0]) + hex_port(host_port_tuple[1])


def unhex_hp(hex_hp_dump):
    """Convert str hex dump to host:port tuple. Example
    str('7F000001B18F') => tuple('127.0.0.1', 45455)
    :param hex_hp_dump:       example '7F000001B18F'
    :return: host:port tuple: example ('127.0.0.1', 45455)
    """
    return '.'.join(
        [str(int(hex_hp_dump[2*j:2*j+2], 16)) for j in xrange(4)]),\
        int(hex_hp_dump[8:], 16)


def extract_port(binary):
    return struct.unpack('!H', binary)[0]


def show_binary_data(title, bytearray_data):
    if type(bytearray_data) != bytearray:
        raise ValueError("Not byte array")
    cout(title)
    cout("".join(["%02X" % b for b in bytearray_data]))
    cout("\n")

stored_transaction_ids = list()


def stun_make_request():  # see RFC 5389 (fixed magic cooke? lol :)
    mt = [0x00, 0x01]              # message type "STUN binding"
    bl = [0x00, 0x00]              # attributes body length = 0
    mc = [0x21, 0x12, 0xA4, 0x42]  # magic cooke (RFC 5389)
    ti = get_random_bytes(12)      # transaction_id (RFC 5389)
    stored_transaction_ids.append(bytearray(mc+ti))
    return bytearray(mt + bl + mc + ti)


def stun_parse_response(data, hpt, verbose=3):
    """Parse Session Traversal Utilities for NAT (STUN) server's response
    :param data: response body: type bytearray()
    :param hpt: sender's host port tuple(): example ('127.0.0.1':45455)
    :param server_info: STUN server record: type dict()
    :param server_name:
    :return: str() hex dump of the public IP:PORT, use 'unhex_hp' to decode.
    Return empty str() if parser failed
    """
    data = bytearray(data)
    show_binary_data("  RECV ", data)
    cout("  FROM %s:%d\n" % hpt)
    # server_info["echo"] = data
    if len(data) < 20:  # (RFC-5389)
        cout("  FAIL %s:%d response is too short\n" % hpt)
        return ""
    if data[0:2] != b'\x01\x01':  # (RFC-5389)
        cout("  FAIL %s:%d STUN non-success response\n" % hpt)
        return ""
    if data[4:20] not in stored_transaction_ids:  # (RFC-5389)
        cout("  FAIL %s:%d STUN bad transaction ID in response\n" % hpt)
        return ""

    # Parse STUN response's body (RFC-5389)
    #
    response_body_len = struct.unpack('!H', data[2:4])[0]
    mc = bytearray(data[4:8])
    if verbose > 2:
        cout("  SIZE %04X\n" % response_body_len)
        show_binary_data("  MAHO ", mc )
        show_binary_data("  T_ID ", data[8:20])
    data_len = response_body_len+20  # (RFC-5389)
    if data_len != len(data):  # (RFC-5389)
        cout("  FAIL %s:%d STUN invalid body size field\n" % hpt)
        return ""
    # parse attributes
    ri = 20
    hpd = ""  # host:port str hex dump
    while ri < data_len:
        if data[ri:ri+2] == b'\00\01':  # MAPPED-ADDRESS (RFC-5389)
            if hpd:  # prevent overwrite XOR-MAPPED-ADDRESS if exists
                continue
            pos = ri+2+2+1+1
            port = data[pos:pos+2]
            host = data[pos+2:pos+2+4]
            hpd = hex_dump(host) + hex_dump(port)
            # mapped_address = hex_dump(host) + hex_dump(port)
            # server_info["mapped_address"] = mapped_address
            if verbose > 2:
                hpt0 = unhex_hp(hpd)
                cout("  MAPPED-ADDRESS     ")
                cout(hex_dump(host) + " " + hex_dump(port) + " ")
                cout(hpt0[0] + ":" + str(hpt0[1]))
                cout("\n")

        elif data[ri:ri+2] == b'\x80\x20' or data[ri:ri+2] == b'\x00\x20':
            # XOR-MAPPED-ADDRESS (RFC-5389)
            pos = ri+2+2+1+1
            port = data[pos:pos+2]
            n = struct.unpack('!H', port)[0]
            m = struct.unpack('!H', mc[0:2])[0]
            port = bytearray(struct.pack('!H', n ^ m))
            host = data[pos+2:pos+2+4]
            n = struct.unpack('!L', host)[0]
            m = struct.unpack('!L', mc)[0]
            host = bytearray(struct.pack('!L', n ^ m))
            hpd = hex_dump(host) + hex_dump(port)
            # mapped_address = hex_dump(host) + hex_dump(port)
            # server_info["xor_mapped_address"] = mapped_address
            if verbose > 2:
                hpt0 = unhex_hp(hpd)
                cout("  XOR-MAPPED-ADDRESS ")
                cout(hex_dump(host) + " " + hex_dump(port) + " ")
                cout(hpt0[0] + ":" + str(hpt0[1]))
                cout("\n")

        elif data[ri:ri+2] == b'\x00\x02':
            if verbose > 2:
                cout("  RESPONSE-ADDRESS   <ignored>\n")

        elif data[ri:ri+2] == b'\x00\x03':
            if verbose > 2:
                cout("  CHANGE-ADDRESS     <ignored>\n")

        elif data[ri:ri+2] == b'\x00\x04':
            if verbose > 2:
                cout("  SOURCE-ADDRESS     <ignored>\n")

        elif data[ri:ri+2] == b'\x00\x05':
            if verbose > 2:
                cout("  CHANGED-ADDRESS    <ignored>\n")

        elif data[ri:ri+2] == b'\x00\x06':
            if verbose > 2:
                cout("  USERNAME           <ignored>\n")

        elif data[ri:ri+2] == b'\x00\x07':
            if verbose > 2:
                cout("  PASSWORD           <ignored>\n")

        elif data[ri:ri+2] == b'\x00\x08':
            if verbose > 2:
                cout("  MESSAGE-INTEGRITY  <ignored>\n")

        elif data[ri:ri+2] == b'\x00\x09':
            if verbose > 2:
                cout("  ERROR-CODE         <ignored>\n")

        elif data[ri:ri+2] == b'\x00\x0A':
            if verbose > 2:
                cout("  UNKNOWN-ATTRIBUTES <ignored>\n")

        elif data[ri:ri+2] == b'\x00\x0B':
            if verbose > 2:
                cout("  REFLECTED-FROM     <ignored>\n")

        elif data[ri:ri+2] == b'\x00\x14':
            if verbose > 2:
                cout("  REALM              <ignored>\n")

        elif data[ri:ri+2] == b'\x00\x15':
            if verbose > 2:
                cout("  NONCE              <ignored>\n")

        elif data[ri:ri+2] == b'\x80\x22':
            if verbose > 2:
                cout("  SOFTWARE           <ignored>\n")

        elif data[ri:ri+2] == b'\x80\x23':
            if verbose > 2:
                cout("  ALTERNATE-SERVER   <ignored>\n")

        elif data[ri:ri+2] == b'\x80\x28':
            if verbose > 2:
                cout("  FINGERPRINT        <ignored>\n")

        else:
            if verbose > 2:  # STDOUT:
                cout("  STUN TYPE 0x%s ?\n" % hex_dump(data[ri:ri+2]))
        # For each attribute include unknown:
        #
        ri += 2
        attrib_value_length = struct.unpack('!H', data[ri:ri+2])[0]
        if attrib_value_length % 4:
            attrib_value_length += 4-(attrib_value_length % 4)  # add padding
        ri += 2
        ri += attrib_value_length

    # if "mapped_address" not in server_info:
    #     if "xor_mapped_address" not in server_info:
    #         cout("  FAIL %s:%d response w/o address info\n" % hpt)
    #         return ""
    if not hpd:
        cout("  FAIL %s:%d response w/o address info\n" % hpt)

    return hpd

    # verbose_servers_dict[server_name] = server_info
    # Use XOR-MAPPED-ADDRESS (RFC-5389) if exists
    # if "xor_mapped_address" in server_info:
    #     return server_info["xor_mapped_address"]
    # return server_info["mapped_address"]


def get_public_hp(server_info, server_name):

    rt = 0.5
    hpt = unhex_hp(server_info["hex"])
    cout("\n" + server_name + " %s\n" % str(hpt))
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_socket.settimeout(0.01)
    buff = 0
    stun_request = stun_make_request()
    for request_number in xrange(3):
        show_binary_data('  SEND ', stun_request)
        udp_socket.sendto(stun_request, hpt)
        wait_response_time = 0
        while wait_response_time < rt:
            try_start_time = time.clock()
            try:
                buff, addr = udp_socket.recvfrom(1280)
                break
            except socket.timeout:
                wait_response_time += time.clock() - try_start_time
                continue
            except socket.error, e:
                cout("  FAIL %s:%d socket.error %s\n" % (hpt + (e,)))
                return ""
        if buff:
            return stun_parse_response(buff, addr)
        rt += rt
    cout("  FAIL %s:%d no response\n" % hpt)
    return ""

if __name__ == "__main__":
    stun_servers_dict = dict()

    cout("\nTEST 'Has valid IP'\n")

    for i in stun_servers_candidates:
        try:
            st = time.clock()
            ip = socket.gethostbyname(i)
            et = 1000 * (time.clock() - st)
        except:
            sys.stdout.write(i+" <FAILED> ")
            sys.stdout.write(sys.exc_info()[0].__name__)
            sys.stdout.write(': %s\n' % str(sys.exc_info()[1]))
            continue
        args = stun_servers_candidates[i]
        args["ip"] = ip
        if not ("port" in args):
            args["port"] = 3478
        args["hex"] = hex_hp((args["ip"], args["port"]))
        stun_servers_dict[i] = args
        cout(i+"  %.3f ms\n" % et)
    cout("\nSUCCESS pass 1 tested:\n")

    keys = stun_servers_dict.keys()
    keys.sort()
    for i in keys:
        cout("%(hex)s " % stun_servers_dict[i]+i+"\n")

    cout("\nSUMMARY has valid ip %d servers from %d candidates\n" %
         (len(stun_servers_dict), len(stun_servers_candidates)))

    cout("\nTEST 'IS STUN servers'\n")

    for ss in stun_servers_dict:
        hp = get_public_hp(stun_servers_dict[ss], ss)
        if hp:
            stun_servers_dict[ss]["hpd"] = hp
            verbose_servers_dict[ss] = stun_servers_dict[ss]

    stun_map = dict()  # final servers dict hex_ip_port:server_info
    #                  # it is for remove duplicated ip:port records
    if len(verbose_servers_dict):  # has valid STUN servers IP:PORT dumps?
        for ss in verbose_servers_dict:
            stun_map[verbose_servers_dict[ss]["hex"]] = ss

    if len(stun_map):  # write results to file, as readable JSON
        f = file("stun.dict.json", "w")
        f.write("{")
        f.write("\n,".join(['"%s":"%s"' % (i, stun_map[i]) for i in stun_map]))
        f.write("\n}\n")

    cout("\n"+str(len(stun_map))+" from ")
    cout(str(len(stun_servers_candidates))+"\n")
