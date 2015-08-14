#ifndef NETWORK_H
#define NETWORK_H

#include "Node.h"
#include "Exceptions.h"

class Network
{
	public:
		Network();
		~Network();
		void addNode(string nodeName);
		void removeNode(string nodeName);
		void addEdge(string source,string destination,int weight);
		void removeEdge(string source,string destination);
		
		friend Network& operator+(Network& l,Network& r);
		friend ostream& operator<<(ostream& stream,Network& network);
		friend bool operator==(Network& l,Network& r);
	private:
		vector<Node *> nodes;
};

#endif
