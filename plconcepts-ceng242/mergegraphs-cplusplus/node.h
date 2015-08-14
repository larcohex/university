#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <vector>
#include "edge.h"
#include "exceptions.h"

using namespace std;

class Edge;

class Node{
	public:
		Node(string label);
		~Node();
		string getLabel();
		void setLabel(string label);
		
		void addEdge(Edge * edge);
		void removeEdge(string destination);
	
		friend Node& operator+(Node& l,Node& r);
		friend bool operator== (Node& l,Node& r);	
	        friend ostream& operator<< (ostream& stream, const Node& node);
	private:
		string label;
		vector<Edge *> adjacencyList;
};

#endif
