from mininet.topo import Topo

class MyTopo( Topo ):
    "Simple topology example."

    def __init__( self, **kwargs ):
        "Create custom topo."
	n = kwargs.get('n', 5)

	switches = range(1,n)
	
        # Initialize topology
        Topo.__init__( self )

        # Add hosts
        self.addHost( 'h1' )
        self.addHost( 'h2' )

        # Add switches
        self.addSwitch( 's0' )
	self.addLink( 'h1' , 's0' )
	for i in switches:
		prev = i - 1
		self.addSwitch( 's%d' % i )
		self.addLink('s%d' % prev, 's%d' % i)
	self.addLink('s%d' % i, 'h2')
	
	

topos = { 'mytopo': ( lambda **args: MyTopo(**args) ) }

