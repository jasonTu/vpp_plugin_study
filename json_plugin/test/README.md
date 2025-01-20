## How to launch the test
```
vpp test:
ip link add name veth_vpp1 type veth peer name vpp1
ip link set dev veth_vpp1 up netns vpp1
ip netns exec vpp1 ifconfig veth_vpp1 10.10.10.2/24
vpp： create host-interface name vpp1
vpp: set interface state host-vpp1 up
vpp: set interface ip address host-vpp1 10.10.10.1/24

ip netns exec vpp1 python land_attack_simulation.py veth_vpp1 10.10.10.1 10.10.10.1
```
