./udpserver 6666&
./mpserver 2000 localhost 6666&
./mpclient 4000 client.sample.conf&
socat udp-listen:4001 udp4:localhost:2000&
socat udp-listen:4002 udp4:localhost:2000&
socat udp-listen:4003 udp4:localhost:2000&
echo "*** run ./udpclient 4000 to test connection ***"
