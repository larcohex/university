#include "network.h"


Network :: Network()
{
}


Network :: ~Network()
{
	for (int i = 0; i < this->nodes.size(); ++i)
	{
		delete this->nodes[i];
	}
	this->nodes.clear();
}



void Network :: addNode (string nodeName)
{
	for (int i = 0; i < this->nodes.size(); ++i)
	{
		if (this->nodes[i]->getLabel() == nodeName)
			throw NODE_ALREADY_EXISTS;
	}
	Node *newNode  = new Node (nodeName);
	this->nodes.push_back (newNode);
}


void Network :: removeNode (string nodeName)
{
	bool deleted = false;
	for (vector <Node *> :: iterator itr = this->nodes.begin(); itr != this->nodes.end(); ++itr)
	{
		if ((*itr)->getLabel() == nodeName)
		{
			Node *toErase = *itr;
			string erName = toErase->getLabel();
			this->nodes.erase (itr);
			delete toErase;
			for (int i = 0; i < this->nodes.size(); ++i)
			{
				try
				{
					this->nodes[i]->removeEdge (erName);
				}
				catch (Exception e)
				{
				}
			}
			deleted = true;
			break;
		}
	}
	if (!deleted)
		throw NODE_DOES_NOT_EXIST;
}


void Network :: addEdge (string source, string destination, int weight)
{
	Node *src, *dest;
	src = dest = NULL;
	for (int i = 0; i < this->nodes.size(); ++i)
	{
		if (this->nodes[i]->getLabel() == source)
			src = this->nodes[i];
		else if (this->nodes[i]->getLabel() == destination)
			dest = this->nodes[i];
	}
	if (!src)
		throw SOURCE_NODE_DOES_NOT_EXIST;
	if (!dest)
		throw DESTINATION_NODE_DOES_NOT_EXIST;
	Edge *newEdge = new Edge (dest, weight);
	src->addEdge (newEdge);
}


void Network :: removeEdge (string source, string destination)
{
	Node *src, *dest;
	src = dest = NULL;
	for (int i = 0; i < this->nodes.size(); ++i)
	{
		if (this->nodes[i]->getLabel() == source)
			src = this->nodes[i];
		else if (this->nodes[i]->getLabel() == destination)
			dest = this->nodes[i];
	}
	if (!src)
		throw SOURCE_NODE_DOES_NOT_EXIST;
	if (!dest)
		throw DESTINATION_NODE_DOES_NOT_EXIST;
	src->removeEdge (destination);
}


Network &operator+ (Network &l, Network &r)
{
	bool exist;
	Network *newNetwork = new Network();
	for(int i = 0 ; i < (int)(l.nodes.size()); ++i)
	{
		exist = false;
		for(int j = 0 ; j < (int)(r.nodes.size()); ++j)
		{
			if(l.nodes[i]->getLabel() == r.nodes[j]->getLabel())
			{
				Node *temp;
				temp = &(*(l.nodes[i]) + *(r.nodes[j]));
				newNetwork->nodes.push_back(temp);
				exist = true;
			} 
		}
		if (!exist)
		{
			newNetwork->nodes.push_back(l.nodes[i]);
		}	
	}
	for(int i = 0 ; i < (int)(r.nodes.size()); ++i)
	{
		exist = false;
		for(int j = 0 ; j < (int)(l.nodes.size()); ++j)
		{
			if(l.nodes[j]->getLabel()== r.nodes[i]->getLabel())
				exist = true;
 		}
		if (!exist)
			newNetwork->nodes.push_back(r.nodes[i]);
	}	
	return *newNetwork;
}


ostream &operator<< (ostream &stream, Network &network)
{
	for (int i = 0; i < network.nodes.size(); ++i)
	{
		stream << *(network.nodes[i]) << endl;
	}
	return stream;
}


bool operator== (Network &l, Network &r)
{
	if (l.nodes.size() != r.nodes.size())
		return false;
	for (int i = 0; i < (int)(l.nodes.size() - 1); ++i)
	{
		for (int j = 0; j < l.nodes.size() - 1 - i; ++j)
		{
			if (l.nodes[j]->getLabel() > l.nodes[j + 1]->getLabel())
			{
				Node *temp = l.nodes[j];
				l.nodes[j] = l.nodes[j + 1];
				l.nodes[j + 1] = temp;
			}
		}
	}
	for (int i = 0; i < (int)(r.nodes.size() - 1); ++i)
	{
		for (int j = 0; j < r.nodes.size() - 1 - i; ++j)
		{
			if (r.nodes[j]->getLabel() > r.nodes[j + 1]->getLabel())
			{
				Node *temp = r.nodes[j];
				r.nodes[j] = r.nodes[j + 1];
				r.nodes[j + 1] = temp;
			}
		}
	}
	for (int i = 0; i < l.nodes.size(); ++i)
	{
		if (!(*(l.nodes[i]) == *(r.nodes[i])))
			return false;
	}
	return true;
}
