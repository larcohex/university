#include "gui.h"



void fillByBreadth (vector <Component *> *result, Component *root, vector <Component *> *queue)
{
	if (!(queue->empty()))
	{
		unsigned int i;
		Component *temp;
		for (i = 0; i < root->getChildren().size(); i++)
		{
			result->push_back (root->getChildren()[i]);
			queue->push_back (root->getChildren()[i]);
		}
		for (i = 0; i < queue->size(); i++)
		{
			queue->erase (queue->begin());
			temp = (*queue)[0];
			fillByBreadth (result, temp, queue);
		}
	}
}




Component *searchByName (string name, Component *root)
{
	if (!root)
	{
		return NULL;
	}
	else
	{
		bool found = false;
		unsigned int i;
		for (i = 0; i < root->getChildren().size(); i++)
		{
			if (root->getChildren()[i]->getName() == name)
			{
				found = true;
				return root;
			}
		}
		if (!found)
		{
			for (i = 0; i < root->getChildren().size(); i++)
			{
				return searchByName (name, root->getChildren()[i]);
			}
		}
	}
}





void fillByArea (vector < pair<string, int> > *pairs, Component *root)
{
	if (root)
	{
		for (unsigned int i = 0; i < root->getChildren().size(); i++)
		{
			fillByArea (pairs, root->getChildren()[i]);
		}
		pair <string, int> toInsert;
		toInsert.first = root->getName();
		toInsert.second = root->getAreaUsage();
		pairs->push_back (toInsert);
	}
}




void addArea (Component *node, int toAdd)
{
	if (node)
	{
		node->setAreaUsage (node->getAreaUsage() + toAdd);
		addArea (node->getParent(), toAdd);
	}
}



void subtractArea (Component *node, int toSubtr)
{
	if (node)
	{
		node->setAreaUsage (node->getAreaUsage() - toSubtr);
		addArea (node->getParent(), toSubtr);
	}
}	

/*=====================================================================================================*/


GUI::GUI (Size size)
{
	Position pos;
	pos.first = 0;
	pos.second = 0;
	this->root = new Component ("root", size, pos);
	this->root->setParent (NULL);
}






Component *GUI::getRoot()
{
	return this->root;
}



const Component *GUI::getRoot() const
{
	const Component *result = this->root;
	return result;
}


Size GUI::getSize()
{
	return this->root->getSize();
}







void GUI::insertComponent (Component *component)
{
	Component *destination;
	destination = contains (component->getPosition());
	destination->addChild (component);
	component->setParent (destination);
	addArea (destination, component->getAreaUsage());
}





bool GUI::removeComponent (string name)
{
	Component *toDelete = searchByName (name, this->root);
	if (toDelete)
	{
		Component *temp = toDelete->getParent();
		for (unsigned int i = 0; i < temp->getChildren().size(); i++)
		{
			if (temp->getChildren()[i] == toDelete)
			{
				temp->getChildren().erase(temp->getChildren().begin() + i);
				subtractArea (temp, toDelete->getAreaUsage());
				return true;
			}
		}
	}
	else
	{
		return false;
	}	
}





Component *GUI::contains (Position point)
{
	vector <Component *> byLevel;
	vector <Component *> queue;
	byLevel.push_back (this->root);
	queue.push_back (this->root);
	fillByBreadth (&byLevel, this->root, &queue);
	queue.clear();
	for (unsigned int i = byLevel.size() - 1; i >= 0; i--)
	{
		if (i != 0)
		{
			if (  ((byLevel[i]->getPosition().first < point.first && point.first < byLevel[i]->getPosition().first + byLevel[i]->getSize().first) && (byLevel[i]->getPosition().second < point.second && point.second < byLevel[i]->getPosition().second + byLevel[i]->getSize().second))   )
			{
				return byLevel[i];
			}
		}
		else
		{
			return byLevel[i];
		}
	}
}







vector < pair<string, int> > GUI::areaUsage()
{
	vector < pair<string, int> > result;
	fillByArea (&result, this->root);
	int totalArea = 0;
	return result;
}






int **GUI::toArray ()
{
	unsigned int i, j, k, l, m;
	int **result = NULL;
	result = new int *[root->getSize().first];
	for (i = 0; i < root->getSize().first; i++)
	{
		result[i] = new int[root->getSize().second];
	}
	vector <Component *> byLevel;
	vector <Component *> queue;
	byLevel.push_back (this->root);
	queue.push_back (this->root);
	fillByBreadth (&byLevel, this->root, &queue);
	queue.clear();
	vector < pair <Component *, int> > assignLevels;
	unsigned int level = 2;
	unsigned int counter = 1;
	pair <Component *, int> newPair;
	newPair.first = byLevel[0];
	newPair.second = 1;
	assignLevels.push_back (newPair);
	for (i = 1; i < byLevel.size(); i++)
	{
		bool levelDown = true;
		for (j = 0; j < counter; j++)
		{
			if (byLevel[i]->getParent() == byLevel[j])
			{
				levelDown = false;
				break;
			}
		}
		if (levelDown)
		{
			++level;
			counter = i;
		}
		newPair.first = byLevel[i];
		newPair.second = level;
		assignLevels.push_back (newPair);
	}
	byLevel.clear();
	for (i = 0; i < this->root->getSize().first; i++)
	{
		for (j = 0; j < this->root->getSize().second; j++)
		{
			if (i == this->root->getPosition().first || i == this->root->getPosition().first + this->root->getSize().first - 1 || j == this->root->getPosition().second || j == this->root->getPosition().second + this->root->getSize().second - 1)
			{
				result[i][j] = 1;
			}
			else
			{
				result[i][j] = 0;
			}
		}
	}








	Position intSec1, intSec2;
	intSec1.first = intSec2.first = -1;
	intSec1.second = intSec2.second = -1;
	for (i = 1; i < assignLevels.size(); i++)
	{
		for (j = assignLevels[i].first->getPosition().first; j < assignLevels[i].first->getPosition().first + assignLevels[i].first->getSize().first; j++)
		{
			for (k = assignLevels[i].first->getPosition().second; k < assignLevels[i].first->getPosition().second + assignLevels[i].first->getSize().second; k++)
			{
				if (j < this->root->getSize().first && k < this->root->getSize().second)
				{
					if (j == assignLevels[i].first->getPosition().first || j == assignLevels[i].first->getPosition().first + assignLevels[i].first->getSize().first - 1 || k == assignLevels[i].first->getPosition().second || k == assignLevels[i].first->getPosition().second + assignLevels[i].first->getSize().second - 1)
					{
						if (result[j][k] == assignLevels[i].second)
						{
							if (intSec1.first == -1)
							{
								intSec1.first = j;
								intSec1.second = k;
							}
							else
							{
								intSec2.first = j;
								intSec2.second = k;
							}
						}
						else if (result[j][k] && result[j][k] < assignLevels[i].second)
						{
							if (assignLevels[i].first->getPosition().first + assignLevels[i].first->getSize().first - 1 < this->root->getSize().first)
							{
								for (l = assignLevels[i].first->getPosition().first + 1; l < assignLevels[i].first->getPosition().first + assignLevels[i].first->getSize().first - 1; l++)
								{
									if (assignLevels[i].first->getPosition().second + assignLevels[i].first->getSize().second - 1 < this->root->getSize().second)
									{
										for (m = assignLevels[i].first->getPosition().second + 1; m < assignLevels[i].first->getPosition().second + assignLevels[i].first->getSize().second - 1; m++)
										{
										result[l][m] = 0;
										}
									}
									else
									{
										for (m = assignLevels[i].first->getPosition().second + 1; m <= root->getSize().second; m++)
										{
										result[l][m] = 0;
										}
									}
								}
							}
							else
							{
								for (l = assignLevels[i].first->getPosition().first + 1; l < root->getSize().first; l++)
								{
									if (assignLevels[i].first->getPosition().second + assignLevels[i].first->getSize().second - 1 < this->root->getSize().second)
									{
										for (m = assignLevels[i].first->getPosition().second + 1; m < assignLevels[i].first->getPosition().second + assignLevels[i].first->getSize().second - 1; m++)
										{
										result[l][m] = 0;
										}
									}
									else
									{
										for (m = assignLevels[i].first->getPosition().second + 1; m < root->getSize().second; m++)
										{
										result[l][m] = 0;
										}
									}
								}
							}
						}
						result[j][k] = assignLevels[i].second;
						if (intSec1.first != -1 && intSec2.first != -1)
						{
							if (intSec1.first < intSec1.first)
							{	
								for (l = intSec1.first; l <= intSec2.first; l++)
								{
									for (m = intSec1.second; m <= intSec2.second; m++)
									{
										result[l][m] = 0;
									}
								}
								intSec1.first = intSec2.first = -1;
								intSec2.second = intSec2.second = -1;
							}
						}
					}
					else
					{
						result[i][j] = 0;
					}
				}
			}
		}
	}
	assignLevels.clear();
	return result;
}






ostream &operator<< (ostream& os, const GUI& gui)
{
	vector <Component *> byLevel;
	vector <Component *> queue;
	Component *tree = const_cast <Component*> (gui.getRoot());
	byLevel.push_back (tree);
	queue.push_back (tree);
	fillByBreadth (&byLevel, tree, &queue);
	queue.clear();
	vector < pair <Component *, int> > assignLevels;
	unsigned int level = 2;
	unsigned int counter = 1;
	pair <Component *, int> newPair;
	newPair.first = byLevel[0];
	newPair.second = 1;
	assignLevels.push_back (newPair);
	unsigned int i, j;
	for (i = 1; i < byLevel.size(); i++)
	{
		bool levelDown = true;
		for (j = 0; j < counter; j++)
		{
			if (byLevel[i]->getParent() == byLevel[j])
			{
				levelDown = false;
				break;
			}
		}
		if (levelDown)
		{
			++level;
			counter = i;
		}
		newPair.first = byLevel[i];
		newPair.second = level;
		assignLevels.push_back (newPair);
	}
	byLevel.clear();
	os << "| root |\n";
	counter = 1;
	for (i = 0; i < assignLevels.size(); i++)
	{
		if (assignLevels[i].second == counter + 1)
		{
			os << endl;
			++counter;
		}
		os << "| ";
		for (j = 0; j < assignLevels[i].first->getChildren().size(); j++)
		{
			os << assignLevels[i].first->getChildren()[j]->getName() << " ";
		}
		os << "|";
	}
	os << endl;
	return os;
}
