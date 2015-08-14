#include "edge.h"


Edge :: Edge (Node *adjacent, int weight)
{
	this->adjacent = adjacent;
	this->weight = weight;
}


Edge :: ~Edge()
{
	this->adjacent = NULL;
}



int Edge :: getWeight()
{
	return this->weight;
}



Node *Edge :: getAdjacent()
{
	return this->adjacent;
}




bool operator== (Edge &l, Edge &r)
{
	if (l.weight == r.weight && *(l.adjacent) == *(r.adjacent))
		return true;
	return false;
}



ostream& operator<< (ostream &stream, const Edge &edge)
{
	stream << "(" << (edge.adjacent)->getLabel() << "," << edge.weight << ")";
}
