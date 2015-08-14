#ifndef EDGE_H
#define EDGE_H

#include <iostream>
#include "node.h"
#include "exceptions.h"

using namespace std;

class Node;

class Edge
{
	public:
		Edge(Node * adjacent,int weight);
		~Edge();

		int getWeight();
		Node * getAdjacent();

		friend bool operator== (Edge& l,Edge& r);
                friend ostream& operator<< (ostream& stream, const Edge& edge);
	private:
		int weight;
		Node * adjacent;
};

#endif
