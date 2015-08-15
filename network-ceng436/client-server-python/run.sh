#!/bin/bash

MN_ROUTER_NUM=5
MN_BANDWIDTH=12
MN_DELAY=1ms
MN_LOSS=10

PING_COUNT=100
PING_PACKET_SIZE=100
PING_INTERVAL=0

if [ ! -f topology.py ];
then
   echo "topology.py file could not be found."
   exit 1;
fi

(expect - << EOD
	spawn sudo mn --custom topology.py --topo mytopo,n=$MN_ROUTER_NUM --link tc,bw=$MN_BANDWIDTH,delay=$MN_DELAY,loss=$MN_LOSS
	set timeout 1000
	
	expect "*mininet>*"
	sleep 1
	send -- "h1 ping h2 -s $PING_PACKET_SIZE -c $PING_COUNT -i $PING_INTERVAL \n"
	
	expect "*mininet>*"
	sleep 1
	send -- "exit\n"

	interact
EOD
) | tee .ping.out | grep -e "ttl=" | awk '{print $(NF-1)" ms"}' | cut -d'=' -f2 > n_${MN_ROUTER_NUM}_BW_${MN_BANDWIDTH}_delay_${MN_DELAY}_loss_${MN_LOSS}.out

cat .ping.out | grep -e "packets transmitted" -e "rtt min/avg/max/mdev"
rm -f .ping.out

echo "Delays are saved in "n_${MN_ROUTER_NUM}_BW_${MN_BANDWIDTH}_delay_${MN_DELAY}_loss_${MN_LOSS}.out

