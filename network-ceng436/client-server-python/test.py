#!/usr/bin/python

from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import CPULimitedHost
from mininet.link import TCLink
from mininet.util import dumpNodeConnections
from mininet.log import setLogLevel
from mininet.node import OVSController
from mininet.cli import CLI

class MultiSwitchTopo(Topo):
        "Multiple switches connected to 2 hosts."
	def __init__(self, n=5, **opts):
		linkopts = dict(bw=10, delay='1ms', loss=1)
		Topo.__init__(self, **opts)
		switches = range(1,n)
		#Add hosts
		h1 = self.addHost( 'h1' )
		h2 = self.addHost( 'h2' )

		# Add switches
		self.addSwitch( 's0' )
		self.addLink( 'h1' , 's0', **linkopts)
		for i in switches:
			prev = i - 1
			self.addSwitch( 's%d' % i )
			self.addLink('s%d' % prev, 's%d' % i, **linkopts)
		self.addLink('s%d' % i, 'h2', **linkopts)

def simpleTest():
        "Create and test a simple network"
	topo = MultiSwitchTopo(n=10)
        net = Mininet(topo, link=TCLink)
        net.start()
        
        h1 = net.get('h1') 
	h2 = net.get('h2')
	print "\n\nping h2 (%s) from h1 (%s) 100 times in a pipelined fashion and print statistics" % (h2.IP(), h1.IP())
	result = h1.cmd('ping -f -c 100 %s' % h2.IP())
	print result
	
	net.stop()

if __name__ == '__main__':
        # Tell mininet to print useful information
        #setLogLevel('info')
        simpleTest()
