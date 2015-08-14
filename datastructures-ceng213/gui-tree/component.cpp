#include "component.h"


Component::Component (string name, Size size, Position position)
{
	this->name = name;
	this->size = size;
	this->position = position;
	this->parent = NULL;
	this->areaUsage = size.first * size.second;
}




string Component::getName()
{
	return this->name;
}



void Component::setName (string name)
{
	this->name = name;
}





Size Component::getSize()
{
	return this->size;
}




void Component::setSize (Size size)
{
	this->size = size;
}




Position Component::getPosition()
{
	return this->position;
}




void Component::setPosition (Position pos)
{
	this->position = pos;
}





Component *Component::getParent()
{
	return this->parent;
}





void Component::setParent (Component *component)
{
	this->parent = component;
}




void Component::addChild (Component *child)
{
	this->children.push_back (child);
}




Component *Component::getChild (string name)
{
	for (unsigned int i = 0; i < (this->children).size(); i++)
	{
		if ((this->children)[i]->getName() == name)
		{
			return (this->children)[i];
			break;
		}
	}
	return NULL;
}





bool Component::removeChild (string name)
{
	bool result = false;
	for (unsigned int i = 0; i < (this->children).size(); i++)
	{
		if ((this->children)[i]->getName() == name)
		{
			(this->children).erase ((this->children).begin() + i);
			result = true;
			break;
		}
	}
	return result;
}








vector <Component *> &Component::getChildren()
{
	return this->children;
}





int Component::getAreaUsage()
{
	return this->areaUsage;
}







void Component::setAreaUsage (int newArea)
{
	this->areaUsage = newArea;
}
