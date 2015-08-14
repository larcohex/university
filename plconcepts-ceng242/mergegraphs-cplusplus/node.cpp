#include "Node.h"


Node :: Node (string label)
{
	this->label = label;
}

Node :: ~Node()
{
	for (int i = 0; i < (int)(this->adjacencyList.size()); ++i)
	{
		delete this->adjacencyList[i];
	}
	this->adjacencyList.clear();
}

string Node :: getLabel()
{
	return this->label;
}

void Node :: setLabel (string label)
{
	this->label = label;
}

void Node :: addEdge (Edge *edge)
{
	for (int i = 0; i < (int)(this->adjacencyList.size()); ++i)
	{
		if (this->adjacencyList[i]->getAdjacent()->getLabel() == edge->getAdjacent()->getLabel())
			throw EDGE_ALREADY_EXISTS;
	}
	this->adjacencyList.push_back (edge);
}

void Node :: removeEdge (string destination)
{
	bool deleted = false;
	if (!this->adjacencyList.empty())
	{
		for (vector <Edge *> :: iterator itr = this->adjacencyList.begin(); itr != this->adjacencyList.end(); ++itr)
		{
			if (((*itr)->getAdjacent())->getLabel() == destination)
			{
				Edge *temp = *itr;
				this->adjacencyList.erase (itr);
				delete temp;
				deleted = true;
				break;
			}
		}
	}
	if (!deleted)
		throw EDGE_DOES_NOT_EXIST;
}

Node &operator+ (Node &l, Node &r) // merges two nodes - for network merging
{
	Node *newNode  = new Node (l.label);
	for (int i = 0; i < (int)(l.adjacencyList.size()); ++i)
	{
		newNode->adjacencyList.push_back (l.adjacencyList[i]);
	}
	for (int i = 0; i < (int)(r.adjacencyList.size()); ++i)
	{
		bool exists = false;
		vector <Edge *> :: iterator itr;
		for (itr = newNode->adjacencyList.begin(); itr != newNode->adjacencyList.end(); ++itr)
		{
			if ((*itr)->getAdjacent()->getLabel() == r.adjacencyList[i]->getAdjacent()->getLabel())
			{
				exists = true;
				break;
			}
		}
		if (!exists)
			newNode->adjacencyList.push_back (r.adjacencyList[i]);
		else
		{
			if ((*itr)->getWeight() < r.adjacencyList[i]->getWeight())
			{
				newNode->adjacencyList.erase (itr++);
				newNode->adjacencyList.insert (itr, r.adjacencyList[i]);
			}
		}
	}		
	return *newNode;
}

bool operator== (Node &l, Node &r)
{
	if (l.label != r.label)
		return false;
	if (l.adjacencyList.size() != r.adjacencyList.size())
		return false;
	vector <Edge *> ltemp = l.adjacencyList;
	vector <Edge *> rtemp = r.adjacencyList;
	for (int i = 0; i < (int)(ltemp.size() - 1); i++)
	{
		for (int j = 0; j < (int)(ltemp.size() - 1 - i); j++)
		{
			if (ltemp[j]->getAdjacent()->getLabel() > ltemp[j + 1]->getAdjacent()->getLabel())
			{
				Edge *temp = ltemp[j + 1];
				ltemp[j + 1] = ltemp[j];
				ltemp[j] = temp;
			}
			else if (ltemp[j]->getAdjacent()->getLabel() == ltemp[j + 1]->getAdjacent()->getLabel() && ltemp[j]->getWeight() > ltemp[j + 1]->getWeight())
			{
				Edge *temp = ltemp[j + 1];
				ltemp[j + 1] = ltemp[j];
				ltemp[j] = temp;
			}
		}
	}
	for (int i = 0; i < (int)(rtemp.size() - 1); i++)
	{
		for (int j = 0; j < (int)(rtemp.size() - 1 - i); j++)
		{
			if (rtemp[j]->getAdjacent()->getLabel() > rtemp[j + 1]->getAdjacent()->getLabel())
			{
				Edge *temp = rtemp[j + 1];
				rtemp[j + 1] = rtemp[j];
				rtemp[j] = temp;
			}
			else if (rtemp[j]->getAdjacent()->getLabel() == rtemp[j + 1]->getAdjacent()->getLabel() && rtemp[j]->getWeight() > rtemp[j + 1]->getWeight())
			{
				Edge *temp = rtemp[j + 1];
				rtemp[j + 1] = rtemp[j];
				rtemp[j] = temp;
			}
		}
	}
	for (int i = 0; i < (int)(ltemp.size()); ++i)
	{
		if (!(*(ltemp[i]) == *(rtemp[i])))
			return false;
	}
	return true;
}

ostream &operator<< (ostream& stream, const Node& node)
{
	if (!(node.adjacencyList.empty()))
	{
		stream << node.label << "[" << *(node.adjacencyList[0]);
		for (int i = 1; i < (int)(node.adjacencyList.size()); ++i)
		{
			stream << "," << *(node.adjacencyList[i]);
		}
		stream << "]";
	}
	else
		stream << node.label << "[]";
	return stream;
}
