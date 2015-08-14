/*
 * main.cpp
 *
 *  Created on: May 18, 2013
 *      Author: Muhammet Kara
 */

#include "mechanics.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <typeinfo>
#include <cmath>
#include <algorithm>

using namespace std;

ostream& operator<< (ostream& stream, const Object& o);
ostream& operator<< (ostream& stream, const vector<Object>& l);
ostream& operator<< (ostream& stream, const MoveType& mType);
ostream& operator<< (ostream& stream, const ObjectType& oType);
vector<Object> generateSightRange(const int range);
void printMap(vector<Object>& l, const int range);
ObjectType getObjType(Creature& c);
int getSightRange(Creature& c);

int main()
{
	/*------------------------Uncomment the one that you want to test--------------------------*/
	Human subject(NORTH, 5, 5);
	//Dwarf subject(NORTH, 5, 5);
	//Dragon subject(NORTH, 5, 5, 5); //with fire left option
	//Dragon subject(NORTH, 5, 5, 0); //no fire left option
	//Cleric subject(NORTH, 5, 5);
	//Wizard subject(NORTH, 5, 5, 5); //Have enough mana to cast spell
	//Wizard subject(NORTH, 5, 5, 0); //No mana, no spell casting
	//BattleMage subject(NORTH, 5, 5, 5); //Have mana and toughness
	//BattleMage subject(NORTH, 5, 5, 0); //No mana, no spell casting
	//BattleMage subject(NORTH, 1, 1, 1); //Low toughness, should heal-self
	/*-----------------------------------------------------------------------------------------*/

	Object sub;
	sub.bewitched = subject.getBewitched();
	sub.objType = getObjType(subject);
	sub.relativeX = 0;
	sub.relativeY = 0;
	sub.teamSide = subject.getSide();

	std::vector<Object> list = generateSightRange(getSightRange(subject));

	list.push_back(sub);
	printMap(list, getSightRange(subject));
	list.pop_back();
	//cout << "list: " << list << endl;
	Move move = subject.move(list);
	cout << "Possible Move Type: " << move.mType << ", RelativeX: " << move.relativeX << ", RelativeY: " << move.relativeY << endl;

	return 0;
}


ostream& operator<< (ostream& stream, const Object& o)
{
	switch(o.objType)
	{
	case DWARF:
		stream << "D";
		break;
	case HUMAN:
		stream << "H";
		break;
	case DRAGON:
		stream << "E";
		break;
	case CLERIC:
		stream << "C";
		break;
	case WIZARD:
		stream << "W";
		break;
	case BATTLEMAGE:
		stream << "B";
		break;
	case WALL:
		stream << "X";
		break;
	case FOOD:
		stream << "F";
		break;
	default:
		stream << "-";
		break;
	}

	return stream;
}

ostream& operator<< (ostream& stream, const Object *o)
{
	switch(o->objType)
	{
	case DWARF:
		stream << "D:" << "(" << o->relativeX << "," << o->relativeY << ") Bewitched:" << o->bewitched;
		break;
	case HUMAN:
		stream << "H:" << "(" << o->relativeX << "," << o->relativeY << ") Bewitched:" << o->bewitched;
		break;
	case DRAGON:
		stream << "E:" << "(" << o->relativeX << "," << o->relativeY << ") Bewitched:" << o->bewitched;
		break;
	case CLERIC:
		stream << "C:" << "(" << o->relativeX << "," << o->relativeY << ") Bewitched:" << o->bewitched;
		break;
	case WIZARD:
		stream << "W:" << "(" << o->relativeX << "," << o->relativeY << ") Bewitched:" << o->bewitched;
		break;
	case BATTLEMAGE:
		stream << "B:" << "(" << o->relativeX << "," << o->relativeY << ") Bewitched:" << o->bewitched;
		break;
	case WALL:
		stream << "X:" << "(" << o->relativeX << "," << o->relativeY << ") Bewitched:" << o->bewitched;
		break;
	case FOOD:
		stream << "F:" << "(" << o->relativeX << "," << o->relativeY << ") Bewitched:" << o->bewitched;
		break;
	default:
		stream << "-";
		break;
	}

	return stream;
}

ostream& operator<< (ostream& stream, const vector<Object>& l)
{
	for(unsigned int i = 0; i < l.size(); i++)
	{
		if (i > 0)
		{
			stream << ", ";
		}
		stream << &(l[i]);
	}
	stream << endl;



	return stream;
}

vector<Object> generateSightRange(const int range)
{
	vector<Object> l;
	Object o;
	int num = 4 * range * range;
	int randMax = (2*range)+1;
	bool occupied;

	srand(time(NULL));
	num = (rand() % num) + range;

	int x, y;

	for (int i = 0; i < num; i++)
	{
		o.bewitched = rand() % 2;
		o.objType = (ObjectType) (rand() % 8);
		o.teamSide = (Side) (rand() % 2);
		while(true)
		{
			x = (rand() % randMax) - range;
			y = (rand() % randMax) - range;

			if (x == 0 && y == 0)
			{
				continue;
			}

			occupied = false;
			for (unsigned int j = 0; j < l.size(); j++) //check if that point is occupied
			{
				if (l[j].relativeX == x && l[j].relativeY == y)
				{
					occupied = true;
					break;
				}
			}

			if (occupied)
			{
				continue;
			}
			else
			{
				o.relativeX = x;
				o.relativeY = y;
				break;
			}
		}

		l.push_back(o); //Add the new occupant to the map
	}

	return l;
}

void printMap(vector<Object>& l, const int range)
{
	bool occupied;
	Side side = l.back().teamSide;
	Side enemySide = (Side)(!side);

	for (int y = range; y >= -range; y--)
	{
		for (int x = -range; x <= range; x++)
		{
			occupied = false;
			for (unsigned int i = 0; i < l.size(); i++)
			{
				if (l[i].relativeX == x && l[i].relativeY == y)
				{
					occupied = true;
					if (l[i].teamSide == side && l[i].objType != FOOD && l[i].objType != WALL)
					{
						cout << "\x1b[32;1m";	//Friend (Green)
					}
					else if (l[i].teamSide == enemySide && l[i].objType != FOOD && l[i].objType != WALL)
					{
						cout << "\x1b[31;1m";	//Enemy (Red)

						if (l[i].bewitched)
						{
							cout << "\x1b[34;1m";	//Frozen enemy (Ice)
						}
					}
					cout << l[i];
					cout << "\x1b[0m" << " ";	//Others in regular color
					break;
				}
			}

			if (!occupied)
			{
				cout << "- ";
			}
		}
		cout << endl;
	}
}

ostream& operator<< (ostream& stream, const MoveType& mType)
{
	switch(mType)
	{
	case WALK:
		stream << "WALK";
		break;
	case HUMANATTACK:
		stream << "HUMANATTACK";
		break;
	case DWARFATTACK:
		stream << "DWARFATTACK";
		break;
	case DRAGONPAW:
		stream << "DRAGONPAW";
		break;
	case DRAGONFIRE:
		stream << "DRAGONFIRE";
		break;
	case HEAL:
		stream << "HEAL";
		break;
	case FREEZESPELL:
		stream << "FREEZESPELL";
		break;
	case WEAKENSPELL:
		stream << "WEAKENSPELL";
		break;
	case NOACT:
		stream << "NOACT";
		break;
	default:
		stream << "UNKNOWN";
		break;
	}

	return stream;
}

ostream& operator<< (ostream& stream, const ObjectType& oType)
{
	switch(oType)
	{
	case HUMAN:
		stream << "HUMAN";
		break;
	case DWARF:
		stream << "DWARF";
		break;
	case DRAGON:
		stream << "DRAGON";
		break;
	case CLERIC:
		stream << "CLERIC";
		break;
	case WIZARD:
		stream << "WIZARD";
		break;
	case BATTLEMAGE:
		stream << "BATTLEMAGE";
		break;
	case WALL:
		stream << "WALL";
		break;
	case FOOD:
		stream << "FOOD";
		break;
	default:
		stream << "UNKNOWN";
		break;
	}

	return stream;
}

//returns range of the creature as int
int getSightRange(Creature& c)
{
	if (typeid(c) == typeid(Human))
		return 2;
	else if (typeid(c) == typeid(Dwarf))
		return 1;
	else if (typeid(c) == typeid(Dragon))
		return 3;
	else if (typeid(c) == typeid(Wizard))
		return 3;
	else if (typeid(c) == typeid(Cleric))
		return 2;
	else if (typeid(c) == typeid(BattleMage))
		return 3;
	else
		return 0;
}

ObjectType getObjType(Creature& c)
{
	if (typeid(c) == typeid(Human))
		return HUMAN;
	else if (typeid(c) == typeid(Dwarf))
		return DWARF;
	else if (typeid(c) == typeid(Dragon))
		return DRAGON;
	else if (typeid(c) == typeid(Wizard))
		return WIZARD;
	else if (typeid(c) == typeid(Cleric))
		return CLERIC;
	else if (typeid(c) == typeid(BattleMage))
		return BATTLEMAGE;
	else
		return WALL;
}
