import dpkt
import socket
import sys

f = open(sys.argv[1], 'rb')
pcap = dpkt.pcap.Reader(f)
sent = {}
received = {}
for ts, buf in pcap:
	# print ts, len(buf)
	try:
		eth = dpkt.ethernet.Ethernet(buf)
		ip = eth.data
		if ip.p == dpkt.ip.IP_PROTO_TCP:
			tcp = ip.data
			if (tcp.flags & dpkt.tcp.TH_SYN) != 0 and (tcp.flags & dpkt.tcp.TH_ACK) == 0:
				src_ip = socket.inet_ntoa(ip.src)
				if src_ip in sent:
					sent[src_ip] += 1
				else:
					sent[src_ip] = 1

			if (tcp.flags & dpkt.tcp.TH_SYN) != 0 and (tcp.flags & dpkt.tcp.TH_ACK) != 0:
				dest_ip = socket.inet_ntoa(ip.dst)
				# print ("dest = " + dest_ip)
				if dest_ip in received:
					received[dest_ip] += 1
				else:
					received[dest_ip] = 1
	except Exception:
		continue

for ip in sent:
	if ip in received:
		if 3 * received[ip] < sent[ip]:
			print ip
	else:
		print ip

