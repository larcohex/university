#include "node.h"
#include "edge.h"
#include "network.h"
#include "exceptions.h"

int main()
{
	Network * x = new Network();
	x->addNode("A");
	x->addNode("B");
	x->addNode("C");

	cout << *x << endl;

	x->removeNode("B");

	cout << *x << endl;

	cout << "------------------" << endl;

	x->addEdge("A","C",4);

	cout << *x << endl;

	cout << "-------EXCEPTIONS---------" << endl;

	try{
		x->addNode("A");
	}
	catch(Exception ex)
	{
		if( ex == NODE_ALREADY_EXISTS )
			cout << "NODE_ALREADY_EXISTS" << endl;
	}

	try{
		x->removeNode("B");
	}
	catch(Exception ex)
	{
		if( ex == NODE_DOES_NOT_EXIST )
			cout << "NODE DOES NOT EXIST" << endl;
	}

	try{
		x->addEdge("A","C",3); // BE AWARE WEIGHT IS DIFFERENT
	}
	catch(Exception ex) {
		if( ex == EDGE_ALREADY_EXISTS )
		{
			cout << "EDGE ALREADY EXISTS" << endl;
		}
	}

	x->removeEdge("A","C");
	cout << *x << endl;

	try{
		x->removeEdge("A","C");
	}
	catch(Exception ex){
		if( ex == EDGE_DOES_NOT_EXIST )
		{
			cout << "EDGE DOES NOT EXIST" << endl;
		}
	}

	try{
		x->addEdge("B","C",1);
	}
	catch(Exception ex)
	{
		if( ex == SOURCE_NODE_DOES_NOT_EXIST )
		{
			cout << "SOURCE NODE DOES NOT EXIST" << endl;
		}
	}

	try{
		x->addEdge("A","B",1);
	}
	catch(Exception ex)
	{
		if( ex == DESTINATION_NODE_DOES_NOT_EXIST )
		{
			cout << "DESTINATION NODE DOES NOT EXIST" << endl;
		}
	}

	cout << "----------- EQUALITY OPERATOR ------------ " << endl;

	Network * y = new Network();
	y->addNode("A");
	y->addNode("C");
	y->addEdge("A","C",5);

	x->addEdge("A","C",5);

	cout << *x << endl;
	cout << * y << endl;
	
	if( *x == *y )
	{
		cout << "PERFECTO" << endl;
	}

	Network * z = new Network();
	z->addNode("A");
	z->addNode("13");
	z->addEdge("A","13",4);

	cout << "BEFORE MERGE" << endl;

	cout << * x << endl;

	cout << "--------" << endl;
	cout << *z << endl;

	Network t = *z + *x;

	cout << "AFTER MERGE" << endl;

	cout << t << endl;
	
	cout << "--------" << endl;
	
	Network p;
	
	p.addNode("V");
	p.addNode("Y");
	
	p.addEdge("V","Y",5);

	cout << p << endl;

	return 0;
}
