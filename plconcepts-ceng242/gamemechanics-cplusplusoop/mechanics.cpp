#include "hw5.h"
#include <iostream>

using namespace std;



int importance (ObjectType o)
{
	switch (o)
	{
		case (BATTLEMAGE):
		{
			return 1;
			break;
		}
		case (WIZARD):
		{
			return 2;
			break;
		}
		case (DRAGON):
		{
			return 3;
			break;
		}
		case (CLERIC):
		{
			return 4;
			break;
		}
		case (DWARF):
		{
			return 5;
			break;
		}
		case (HUMAN):
		{
			return 6;
			break;
		}
		case (FOOD):
		{
			return 7;
			break;
		}
		case (WALL):
		{
			return 8;
			break;
		}
	}
}


int abs (int n)
{
	int mask = (n >> 31);
	return (n ^ mask) - mask;
}


void mergeUnits3 (vector <Object> &v, int begin, int end)
{
	vector <Object> sv;
	int middle = (begin + end) / 2;
	int i = begin, j = middle + 1;
	while (i <= middle && j <= end)
	{
		if (abs (v[i].relativeX) < 3 && abs (v[i].relativeY) < 3)
		{
			if (abs (v[j].relativeX) < 3 && abs (v[j].relativeY) < 3)
			{
				if (importance (v[i].objType) < importance (v[j].objType))
				{
					sv.push_back (v[i]);
					++i;
				}
				else if (importance (v[i].objType) > importance (v[j].objType))
				{
					sv.push_back (v[j]);
					++j;
				}
				else
				{
					if (abs (v[i].relativeX) + abs (v[i].relativeY) < abs (v[j].relativeX) + abs (v[j].relativeY))
					{
						sv.push_back (v[i]);
						++i;
					}
					else
					{
						sv.push_back (v[j]);
						++j;
					}
				}
			}
			else
			{
				sv.push_back (v[i]);
				++i;
			}
		}
		else
		{
			if (abs (v[j].relativeX) < 3 && abs (v[j].relativeY) < 3)
			{
				sv.push_back (v[j]);
				++j;
			}
			else
			{
				sv.push_back (v[i]);
				++i;
			}
		}
	}
	while (i <= middle)
	{
		sv.push_back (v[i]);
		++i;
	}
	while (j <= end)
	{
		sv.push_back (v[j]);
		++j;
	}
	for (i = begin; i <= end; ++i)
	{
		v[i] = sv[i - begin];
	}
}


void sortUnits3 (vector <Object> &v, int begin, int end)
{
	if (end > begin)
	{
		int middle = (begin + end) / 2;
		sortUnits3 (v, begin, middle);
		sortUnits3 (v, middle + 1, end);
		mergeUnits3 (v, begin, end);
	}
}


void mergeUnits2 (vector <Object> &v, int begin, int end)
{
	vector <Object> sv;
	int middle = (begin + end) / 2;
	int i = begin, j = middle + 1;
	while (i <= middle && j <= end)
	{
		if (abs (v[i].relativeX) < 4 && abs (v[i].relativeY) < 4)
		{
			if (abs (v[j].relativeX) < 4 && abs (v[j].relativeY) < 4)
			{
				if (importance (v[i].objType) < importance (v[j].objType))
				{
					sv.push_back (v[i]);
					++i;
				}
				else if (importance (v[i].objType) > importance (v[j].objType))
				{
					sv.push_back (v[j]);
					++j;
				}
				else
				{
					if (abs (v[i].relativeX) + abs (v[i].relativeY) < abs (v[j].relativeX) + abs (v[j].relativeY))
					{
						sv.push_back (v[i]);
						++i;
					}
					else
					{
						sv.push_back (v[j]);
						++j;
					}
				}
			}
			else
			{
				sv.push_back (v[i]);
				++i;
			}
		}
		else
		{
			if (abs (v[j].relativeX) < 4 && abs (v[j].relativeY) < 4)
			{
				sv.push_back (v[j]);
				++j;
			}
			else
			{
				sv.push_back (v[i]);
				++i;
			}
		}
	}
	while (i <= middle)
	{
		sv.push_back (v[i]);
		++i;
	}
	while (j <= end)
	{
		sv.push_back (v[j]);
		++j;
	}
	for (i = begin; i <= end; ++i)
	{
		v[i] = sv[i - begin];
	}
}


void sortUnits2 (vector <Object> &v, int begin, int end)
{
	if (end > begin)
	{
		int middle = (begin + end) / 2;
		sortUnits2 (v, begin, middle);
		sortUnits2 (v, middle + 1, end);
		mergeUnits2 (v, begin, end);
	}
}

void mergeUnits (vector <Object> &v, int begin, int end)
{
	vector <Object> sv;
	int middle = (begin + end) / 2;
	int i = begin, j = middle + 1;
	while (i <= middle && j <= end)
	{
		if ((abs (v[i].relativeX) >= 0 && abs (v[i].relativeX) < 2 && abs (v[i].relativeY) >= 0 && abs (v[i].relativeY) < 2) && (abs (v[j].relativeX) > 1 || abs (v[j].relativeY) > 1))
		{
			sv.push_back (v[i]);
			++i;
		}
		else if ((abs (v[j].relativeX) >= 0 && abs (v[j].relativeX) < 2 && abs (v[j].relativeY) >= 0 && abs (v[j].relativeY) < 2) && (abs (v[i].relativeX) > 1 || abs (v[i].relativeY) > 1))
		{
			sv.push_back (v[j]);
			++j;
		}
		else if ((abs (v[i].relativeX) >= 0 && abs (v[i].relativeX) < 2 && abs (v[i].relativeY) >= 0 && abs (v[i].relativeY) < 2) && (abs (v[j].relativeX) >= 0 && abs (v[j].relativeX) < 2 && abs (v[j].relativeY) >= 0 && abs (v[j].relativeY) < 2))
		{
			if (importance (v[i].objType) < importance (v[j].objType))
			{
				sv.push_back (v[i]);
				++i;
			}
			else
			{
				sv.push_back (v[j]);
				++j;
			}
		}
		else
		{
			if (importance (v[i].objType) < importance (v[j].objType))
			{
				sv.push_back (v[i]);
				++i;
			}
			else if (importance (v[i].objType) > importance (v[j].objType))
			{
				sv.push_back (v[j]);
				++j;
			}
			else
			{
				if (abs (v[i].relativeX) + abs (v[i].relativeY) < abs (v[j].relativeX) + abs (v[j].relativeY))
				{
					sv.push_back (v[i]);
					++i;
				}
				else
				{
					sv.push_back (v[j]);
					++j;
				}
			}
		}
	}
	while (i <= middle)
	{
		sv.push_back (v[i]);
		++i;
	}
	while (j <= end)
	{
		sv.push_back (v[j]);
		++j;
	}
	for (i = begin; i <= end; ++i)
	{
		v[i] = sv[i - begin];
	}
}



void sortUnits (vector <Object> &v, int begin, int end)
{
	if (end > begin)
	{
		int middle = (begin + end) / 2;
		sortUnits (v, begin, middle);
		sortUnits (v, middle + 1, end);
		mergeUnits (v, begin, end);
	}
}



void mergeFood (vector <Object> &v, int begin, int end)
{
	vector <Object> sv;
	int middle = (begin + end) / 2;
	int i = begin, j = middle + 1;
	while (i <= middle && j <= end)
	{
		if (abs (v[i].relativeX) >= 0 && abs (v[i].relativeX) < 2 && abs (v[i].relativeY) >= 0 && abs (v[i].relativeY) < 2)
		{
			sv.push_back (v[i]);
			++i;
		}
		else if (abs (v[j].relativeX) >= 0 && abs (v[j].relativeX) < 2 && abs (v[j].relativeY) >= 0 && abs (v[j].relativeY) < 2)
		{
			sv.push_back (v[j]);
			++j;
		}
		else
		{
			if ((abs (v[i].relativeX) + abs (v[i].relativeY)) < (abs (v[j].relativeX) + abs (v[j].relativeY)))
			{
				sv.push_back (v[i]);
				++i;
			}
			else
			{
				sv.push_back (v[j]);
				++j;
			}
		}
	}
	while (i <= middle)
	{
		sv.push_back (v[i]);
		++i;
	}
	while (j <= end)
	{
		sv.push_back (v[j]);
		++j;
	}
	for (i = begin; i <= end; ++i)
	{
		v[i] = sv[i - begin];
	}
}



void sortFood (vector <Object> &v, int begin, int end)
{
	if (end > begin)
	{
		int middle = (begin + end) / 2;
		sortFood (v, begin, middle);
		sortFood (v, middle + 1, end);
		mergeFood (v, begin, end);
	}
}



bool isFriend (vector <Object> &v, int x, int y)
{
	for (int i = 0; i < v.size(); ++i)
	{
		if (v[i].relativeX == x && v[i].relativeY == y)
		{
			return true;
		}
	}
	return false;
}



bool isEnemy (vector <Object> &v, int x, int y)
{
	for (int i = 0; i < v.size(); ++i)
	{
		if (v[i].relativeX == x && v[i].relativeY == y)
		{
			return true;
		}
	}
	return false;
}



bool isWall (vector <Object> &v, int x, int y)
{
	for (int i = 0; i < v.size(); ++i)
	{
		if (v[i].relativeX == x && v[i].relativeY == y)
		{
			return true;
		}
	}
	return false;
}


bool isFood (vector <Object> &v, int x, int y)
{
	for (int i = 0; i < v.size(); ++i)
	{
		if (v[i].relativeX == x && v[i].relativeY == y)
		{
			return true;
		}
	}
	return false;
}

/* battlemage > wizard > dragon > cleric > dwarf > human */

/* DON'T ATTACK BEWITCHED ONES */

/*

			HUMAN
	Sight: 2
	Act: 1
	Actions: Human Attack, Walk, No Act
	Priority: Attack > Move closer to enemy > Food > Move closer to food > Move randomly > No act

*/


void Human :: operator++()
{
	this->loseToughness (-1);
}


Move Human :: move (const vector <Object> &list)
{
	Move next;
	if (this->getBewitched())
	{
		next.mType = NOACT;
		next.relativeX = 0;
		next.relativeY = 0;
		return next;
	}
	vector <Object> enemies, food, walls, friends;
	for (int i = 0; i < list.size(); ++i)
	{
		if (list[i].objType == FOOD)
			food.push_back (list[i]);
		else if (list[i].objType == WALL)
			walls.push_back (list[i]);
		else if (list[i].teamSide != this->getSide())
			enemies.push_back (list[i]);
		else if (list[i].teamSide == this->getSide())
			friends.push_back (list[i]);
	}
	sortUnits (enemies, 0, enemies.size() - 1);


	/*		Attacking enemy			*/

	for (int i = 0; i < enemies.size(); ++i)
	{
		if (abs (enemies[i].relativeX) >= 0 && abs (enemies[i].relativeX) < 2 && abs (enemies[i].relativeY) >= 0 && abs (enemies[i].relativeY) < 2)
		{
			if (!enemies[i].bewitched)
			{
				next.mType = HUMANATTACK;
				next.relativeX = enemies[i].relativeX;
				next.relativeY = enemies[i].relativeY;
				return next;
			}
		}
		else
			break;
	}

	sortUnits3 (enemies, 0, enemies.size() - 1);

	/*		Moving closer to enemy		*/

	for (int i = 0; i < enemies.size(); ++i)
	{
		if (abs (enemies[i].relativeX) + abs (enemies[i].relativeY) < 5)
		{
			if (abs (enemies[i].relativeX) < 3 && abs (enemies[i].relativeY) < 3)
			{
				if (enemies[i].relativeX < 0)
				{
					if (enemies[i].relativeY < 0)
					{
						if (!isEnemy (enemies, -1, -1) && !isFriend (friends, -1, -1) && !isWall (walls, -1, -1))
						{
							if (isFood (food, -1, -1))
								this->loseToughness (-1);
							next.mType = WALK;
							next.relativeX = -1;
							next.relativeY = -1;
							return next;
						}
						else if (!isEnemy (enemies, -1, 0) && !isFriend (friends, -1, 0) && !isWall (walls, -1, 0))
						{
							if (isFood (food, -1, 0))
								this->loseToughness (-1);
							next.mType = WALK;
							next.relativeX = -1;
							next.relativeY = 0;
							return next;
						}
						else if (!isEnemy (enemies, 0, -1) && !isFriend (friends, 0, -1) && !isWall (walls, 0, -1))
						{
							if (isFood (food, 0, -1))
								this->loseToughness (-1);
							next.mType = WALK;
							next.relativeX = 0;
							next.relativeY = -1;
							return next;
						}
					}
					else if (enemies[i].relativeY == 0)
					{
						if (!isEnemy (enemies, -1, 0) && !isFriend (friends, -1, 0) && !isWall (walls, -1, 0))
						{
							if (isFood (food, -1, 0))
								this->loseToughness (-1);
							next.mType = WALK;
							next.relativeX = -1;
							next.relativeY = 0;
							return next;
						}
					}
					else
					{
						if (!isEnemy (enemies, -1, 1) && !isFriend (friends, -1, 1) && !isWall (walls, -1, 1))
						{
							if (isFood (food, -1, 1))
								this->loseToughness (-1);
							next.mType = WALK;
							next.relativeX = -1;
							next.relativeY = 1;
							return next;
						}
						else if (!isEnemy (enemies, -1, 0) && !isFriend (friends, -1, 0) && !isWall (walls, -1, 0))
						{
							if (isFood (food, -1, 0))
								this->loseToughness (-1);
							next.mType = WALK;
							next.relativeX = -1;
							next.relativeY = 0;
							return next;
						}
						else if (!isEnemy (enemies, 0, 1) && !isFriend (friends, 0, 1) && !isWall (walls, 0, 1))
						{
							if (isFood (food, 0, 1))
								this->loseToughness (-1);
							next.mType = WALK;
							next.relativeX = 0;
							next.relativeY = 1;
							return next;
						}
					}
				}
				else if (enemies[i].relativeX == 0)
				{
					if (enemies[i].relativeY < 0)
					{
						if (!isEnemy (enemies, 0, -1) && !isFriend (friends, 0, -1) && !isWall (walls, 0, -1))
						{
							if (isFood (food, 0, -1))
								this->loseToughness (-1);
							next.mType = WALK;
							next.relativeX = 0;
							next.relativeY = -1;
							return next;
						}
					}
					else
					{
						if (!isEnemy (enemies, 0, 1) && !isFriend (friends, 0, 1) && !isWall (walls, 0, 1))
						{
							if (isFood (food, 0, 1))
								this->loseToughness (-1);
							next.mType = WALK;
							next.relativeX = 0;
							next.relativeY = 1;
							return next;
						}
					}
				}
				else
				{
					if (enemies[i].relativeY < 0)
					{
						if (!isEnemy (enemies, 1, -1) && !isFriend (friends, 1, -1) && !isWall (walls, 1, -1))
						{
							if (isFood (food, 1, -1))
								this->loseToughness (-1);
							next.mType = WALK;
							next.relativeX = 1;
							next.relativeY = -1;
							return next;
						}
						else if (!isEnemy (enemies, 1, 0) && !isFriend (friends, 1, 0) && !isWall (walls, 1, 0))
						{
							if (isFood (food, 1, 0))
								this->loseToughness (-1);
							next.mType = WALK;
							next.relativeX = 1;
							next.relativeY = 0;
							return next;
						}
						else if (!isEnemy (enemies, 0, -1) && !isFriend (friends, 0, -1) && !isWall (walls, 0, -1))
						{
							if (isFood (food, 0, -1))
								this->loseToughness (-1);
							next.mType = WALK;
							next.relativeX = 0;
							next.relativeY = -1;
							return next;
						}
					}
					else if (enemies[i].relativeY == 0)
					{
						if (!isEnemy (enemies, 1, 0) && !isFriend (friends, 1, 0) && !isWall (walls, 1, 0))
						{
							if (isFood (food, 1, 0))
								this->loseToughness (-1);
							next.mType = WALK;
							next.relativeX = 1;
							next.relativeY = 0;
							return next;
						}
					}
					else
					{
						if (!isEnemy (enemies, 1, 1) && !isFriend (friends, 1, 1) && !isWall (walls, 1, 1))
						{
							if (isFood (food, 1, 1))
								this->loseToughness (-1);
							next.mType = WALK;
							next.relativeX = 1;
							next.relativeY = 1;
							return next;
						}
						else if (!isEnemy (enemies, 1, 0) && !isFriend (friends, 1, 0) && !isWall (walls, 1, 0))
						{
							if (isFood (food, 1, 0))
								this->loseToughness (-1);
							next.mType = WALK;
							next.relativeX = 1;
							next.relativeY = 0;
							return next;
						}
						else if (!isEnemy (enemies, 0, 1) && !isFriend (friends, 0, 1) && !isWall (walls, 0, 1))
						{
							if (isFood (food, 0, 1))
								this->loseToughness (-1);
							next.mType = WALK;
							next.relativeX = 0;
							next.relativeY = 1;
							return next;
						}
					}
				}
			}
		}
		else
			break;
	}


	/*		Eating food		*/

	sortFood (food, 0, food.size() - 1);
	for (int i = 0; i < food.size(); ++i)
	{
		if (abs (food[i].relativeX) >= 0 && abs (food[i].relativeX) < 2 && abs (food[i].relativeY) >= 0 && abs (food[i].relativeY) < 2)
		{
			this->loseToughness (-1);
			next.mType = WALK;
			next.relativeX = food[i].relativeX;
			next.relativeY = food[i].relativeY;
			return next;
		}
		else
			break;
	}


	/*		Moving closer to food		*/

	for (int i = 0; i < food.size(); ++i)
	{
		if (abs (food[i].relativeX) + abs (food[i].relativeY) < 5)
		{
			if (abs (food[i].relativeX) < 3 && abs (food[i].relativeY) < 3)
			{
				if (food[i].relativeX < 0)
				{
					if (food[i].relativeY < 0)
					{
						if (!isEnemy (enemies, -1, -1) && !isFriend (friends, -1, -1) && !isWall (walls, -1, -1))
						{
							next.mType = WALK;
							next.relativeX = -1;
							next.relativeY = -1;
							return next;
						}
						else if (!isEnemy (enemies, -1, 0) && !isFriend (friends, -1, 0) && !isWall (walls, -1, 0))
						{
							next.mType = WALK;
							next.relativeX = -1;
							next.relativeY = 0;
							return next;
						}
						else if (!isEnemy (enemies, 0, -1) && !isFriend (friends, 0, -1) && !isWall (walls, 0, -1))
						{
							next.mType = WALK;
							next.relativeX = 0;
							next.relativeY = -1;
							return next;
						}
					}
					else if (food[i].relativeY == 0)
					{
						if (!isEnemy (enemies, -1, 0) && !isFriend (friends, -1, 0) && !isWall (walls, -1, 0))
						{
							next.mType = WALK;
							next.relativeX = -1;
							next.relativeY = 0;
							return next;
						}
					}
					else
					{
						if (!isEnemy (enemies, -1, 1) && !isFriend (friends, -1, 1) && !isWall (walls, -1, 1))
						{
							next.mType = WALK;
							next.relativeX = -1;
							next.relativeY = 1;
							return next;
						}
						else if (!isEnemy (enemies, -1, 0) && !isFriend (friends, -1, 0) && !isWall (walls, -1, 0))
						{
							next.mType = WALK;
							next.relativeX = -1;
							next.relativeY = 0;
							return next;
						}
						else if (!isEnemy (enemies, 0, 1) && !isFriend (friends, 0, 1) && !isWall (walls, 0, 1))
						{
							next.mType = WALK;
							next.relativeX = 0;
							next.relativeY = 1;
							return next;
						}
					}
				}
				else if (food[i].relativeX == 0)
				{
					if (food[i].relativeY < 0)
					{
						if (!isEnemy (enemies, 0, -1) && !isFriend (friends, 0, -1) && !isWall (walls, 0, -1))
						{
							next.mType = WALK;
							next.relativeX = 0;
							next.relativeY = -1;
							return next;
						}
					}
					else
					{
						if (!isEnemy (enemies, 0, 1) && !isFriend (friends, 0, 1) && !isWall (walls, 0, 1))
						{
							next.mType = WALK;
							next.relativeX = 0;
							next.relativeY = 1;
							return next;
						}
					}
				}
				else
				{
					if (food[i].relativeY < 0)
					{
						if (!isEnemy (enemies, 1, -1) && !isFriend (friends, 1, -1) && !isWall (walls, 1, -1))
						{
							next.mType = WALK;
							next.relativeX = 1;
							next.relativeY = -1;
							return next;
						}
						else if (!isEnemy (enemies, 1, 0) && !isFriend (friends, 1, 0) && !isWall (walls, 1, 0))
						{
							next.mType = WALK;
							next.relativeX = 1;
							next.relativeY = 0;
							return next;
						}
						else if (!isEnemy (enemies, 0, -1) && !isFriend (friends, 0, -1) && !isWall (walls, 0, -1))
						{
							next.mType = WALK;
							next.relativeX = 0;
							next.relativeY = -1;
							return next;
						}
					}
					else if (food[i].relativeY == 0)
					{
						if (!isEnemy (enemies, 1, 0) && !isFriend (friends, 1, 0) && !isWall (walls, 1, 0))
						{
							next.mType = WALK;
							next.relativeX = 1;
							next.relativeY = 0;
							return next;
						}
					}
					else
					{
						if (!isEnemy (enemies, 1, 1) && !isFriend (friends, 1, 1) && !isWall (walls, 1, 1))
						{
							next.mType = WALK;
							next.relativeX = 1;
							next.relativeY = 1;
							return next;
						}
						else if (!isEnemy (enemies, 1, 0) && !isFriend (friends, 1, 0) && !isWall (walls, 1, 0))
						{
							next.mType = WALK;
							next.relativeX = 1;
							next.relativeY = 0;
							return next;
						}
						else if (!isEnemy (enemies, 0, 1) && !isFriend (friends, 0, 1) && !isWall (walls, 0, 1))
						{
							next.mType = WALK;
							next.relativeX = 0;
							next.relativeY = 1;
							return next;
						}
					}
				}
			}
		}
		else
			break;
	}



	/*		Moving randomly			*/

	if (!isEnemy (enemies, -1, -1) && !isFriend (friends, -1, -1) && !isWall (walls, -1, -1))
	{
		next.mType = WALK;
		next.relativeX = -1;
		next.relativeY = -1;
		return next;
	}
	else if (!isEnemy (enemies, -1, 0) && !isFriend (friends, -1, 0) && !isWall (walls, -1, 0))
	{
		next.mType = WALK;
		next.relativeX = -1;
		next.relativeY = 0;
		return next;
	}
	else if (!isEnemy (enemies, 0, -1) && !isFriend (friends, 0, -1) && !isWall (walls, 0, -1))
	{
		next.mType = WALK;
		next.relativeX = 0;
		next.relativeY = -1;
		return next;
	}
	else if (!isEnemy (enemies, 1, -1) && !isFriend (friends, 1, -1) && !isWall (walls, 1, -1))
	{
		next.mType = WALK;
		next.relativeX = 1;
		next.relativeY = -1;
		return next;
	}
	else if (!isEnemy (enemies, 1, 0) && !isFriend (friends, 1, 0) && !isWall (walls, 1, 0))
	{
		next.mType = WALK;
		next.relativeX = 1;
		next.relativeY = 0;
		return next;
	}
	else if (!isEnemy (enemies, 1, 1) && !isFriend (friends, 1, 1) && !isWall (walls, 1, 1))
	{
		next.mType = WALK;
		next.relativeX = 1;
		next.relativeY = 1;
		return next;
	}
	else if (!isEnemy (enemies, 0, 1) && !isFriend (friends, 0, 1) && !isWall (walls, 0, 1))
	{
		next.mType = WALK;
		next.relativeX = 0;
		next.relativeY = 1;
		return next;
	}
	else if (!isEnemy (enemies, -1, 1) && !isFriend (friends, -1, 1) && !isWall (walls, -1, 1))
	{
		next.mType = WALK;
		next.relativeX = -1;
		next.relativeY = 1;
		return next;
	}


	/*		No action		*/

	next.mType = NOACT;
	next.relativeX = 0;
	next.relativeY = 0;
	return next;
}








/*

			DWARF
	Sight: 1
	Act: 1
	Actions: Dwarf Attack, Walk, No Act
	Priority: Attack > Move closer to enemy > Food > Move closer to food > Move randomly > No act

*/


void Dwarf :: operator++()
{
	this->loseToughness (-2);
}


Move Dwarf :: move (const vector <Object> &list)
{
	Move next;
	vector <Object> enemies, food, walls, friends;
	for (int i = 0; i < list.size(); ++i)
	{
		if (list[i].objType == FOOD)
			food.push_back (list[i]);
		else if (list[i].objType == WALL)
			walls.push_back (list[i]);
		else if (list[i].teamSide != this->getSide())
			enemies.push_back (list[i]);
		else if (list[i].teamSide == this->getSide())
			friends.push_back (list[i]);
	}
	sortUnits (enemies, 0, enemies.size() - 1);



	/*		Attacking enemy			*/

	for (int i = 0; i < enemies.size(); ++i)
	{
		if (abs (enemies[i].relativeX) >= 0 && abs (enemies[i].relativeX) < 2 && abs (enemies[i].relativeY) >= 0 && abs (enemies[i].relativeY) < 2)
		{
			if (!enemies[i].bewitched)
			{
				next.mType = DWARFATTACK;
				next.relativeX = enemies[i].relativeX;
				next.relativeY = enemies[i].relativeY;
				return next;
			}
		}
		else
			break;
	}


	/*		Eating food		*/

	for (int i = 0; i < food.size(); ++i)
	{
		if (abs (food[i].relativeX) >= 0 && abs (food[i].relativeX) < 2 && abs (food[i].relativeY) >= 0 && abs (food[i].relativeY) < 2)
		{
			this->loseToughness (-1);
			next.mType = WALK;
			next.relativeX = food[i].relativeX;
			next.relativeY = food[i].relativeY;
			return next;
		}
		else
			break;
	}



	/*		Moving randomly			*/

	if (!isEnemy (enemies, -1, -1) && !isFriend (friends, -1, -1) && !isWall (walls, -1, -1))
	{
		next.mType = WALK;
		next.relativeX = -1;
		next.relativeY = -1;
		return next;
	}
	else if (!isEnemy (enemies, -1, 0) && !isFriend (friends, -1, 0) && !isWall (walls, -1, 0))
	{
		next.mType = WALK;
		next.relativeX = -1;
		next.relativeY = 0;
		return next;
	}
	else if (!isEnemy (enemies, 0, -1) && !isFriend (friends, 0, -1) && !isWall (walls, 0, -1))
	{
		next.mType = WALK;
		next.relativeX = 0;
		next.relativeY = -1;
		return next;
	}
	else if (!isEnemy (enemies, 1, -1) && !isFriend (friends, 1, -1) && !isWall (walls, 1, -1))
	{
		next.mType = WALK;
		next.relativeX = 1;
		next.relativeY = -1;
		return next;
	}
	else if (!isEnemy (enemies, 1, 0) && !isFriend (friends, 1, 0) && !isWall (walls, 1, 0))
	{
		next.mType = WALK;
		next.relativeX = 1;
		next.relativeY = 0;
		return next;
	}
	else if (!isEnemy (enemies, 1, 1) && !isFriend (friends, 1, 1) && !isWall (walls, 1, 1))
	{
		next.mType = WALK;
		next.relativeX = 1;
		next.relativeY = 1;
		return next;
	}
	else if (!isEnemy (enemies, 0, 1) && !isFriend (friends, 0, 1) && !isWall (walls, 0, 1))
	{
		next.mType = WALK;
		next.relativeX = 0;
		next.relativeY = 1;
		return next;
	}
	else if (!isEnemy (enemies, -1, 1) && !isFriend (friends, -1, 1) && !isWall (walls, -1, 1))
	{
		next.mType = WALK;
		next.relativeX = -1;
		next.relativeY = 1;
		return next;
	}


	/*		No action		*/

	next.mType = NOACT;
	next.relativeX = 0;
	next.relativeY = 0;
	return next;
}







/*

			DRAGON
	Sight: 3
	Act: 1 - 3
	Actions: Dragon Paw, Dragon Fire, Walk, No Act
	Priority: Dragon Paw > Dragon Fire > Move closer to enemy > Food > Move closer to food > Move randomly > No act
	NOTE: Dwarfs cannot be attacked with Dragon Fire

*/


int Dragon :: getLeftFireCount() const
{
	return this->leftFireCount;
}


void Dragon :: operator++()
{
	this->loseToughness (-1);
	++this->leftFireCount;
}


Move Dragon :: move (const vector <Object> &list)
{
	Move next;
	vector <Object> enemies, food, walls, friends;
	for (int i = 0; i < list.size(); ++i)
	{
		if (list[i].objType == FOOD)
			food.push_back (list[i]);
		else if (list[i].objType == WALL)
			walls.push_back (list[i]);
		else if (list[i].teamSide != this->getSide())
			enemies.push_back (list[i]);
		else if (list[i].teamSide == this->getSide())
			friends.push_back (list[i]);
	}
	sortUnits (enemies, 0, enemies.size() - 1);


	/*		Dragon paw		*/

	for (int i = 0; i < enemies.size(); ++i)
	{
		if (!enemies[i].bewitched && (abs (enemies[i].relativeX) >= 0 && abs (enemies[i].relativeX) < 2 && abs (enemies[i].relativeY) >= 0 && abs (enemies[i].relativeY) < 2))
		{
			next.mType = DRAGONPAW;
			next.relativeX = enemies[i].relativeX;
			next.relativeY = enemies[i].relativeY;
			return next;
		}
	}


	/*		Dragon fire		*/

	if (this->leftFireCount)
	{
		for (int i = 0; i < enemies.size(); ++i)
		{
			if (!enemies[i].bewitched && enemies[i].objType != DWARF && (abs (enemies[i].relativeX) < 4 && abs (enemies[i].relativeY) < 4))
			{
				next.mType = DRAGONFIRE;
				next.relativeX = enemies[i].relativeX;
				next.relativeY = enemies[i].relativeY;
				return next;
			}
		}
	}

	
	sortUnits2 (enemies, 0, enemies.size() - 1);

	/*		Moving closer to enemy		*/

	for (int i = 0; i < enemies.size(); ++i)
	{
		if (abs (enemies[i].relativeX) + abs (enemies[i].relativeY) < 7)
		{
			if (abs (enemies[i].relativeX) < 4 && abs (enemies[i].relativeY) < 4)
			{
				int dist = abs (enemies[i].relativeX) + abs (enemies[i].relativeY);
				for (int j = 1; j < dist; ++j)
				{
					for (int k = 0; k <= j; ++k)
					{
						int temp1 = enemies[i].relativeX + j - k, temp2 = enemies[i].relativeY + k;
						if (temp1 || temp2)
						{
							if (temp1 < 4 && temp2 < 4)
							{
								if (!isEnemy (enemies, temp1, temp2) && !isFriend (friends, temp1, temp2) && !isWall (walls, temp1, temp2))
								{
									if (isFood (food, temp1, temp2))
										this->loseToughness (-1);
									next.mType = WALK;
									next.relativeX = temp1;
									next.relativeY = temp2;
									return next;
								}
							}
						}
						temp1 = enemies[i].relativeX + j - k;
						temp2 = enemies[i].relativeY - k;
						if (temp1 || temp2)
						{
							if (temp1 < 4 && temp2 > -4)
							{
								if (!isEnemy (enemies, temp1, temp2) && !isFriend (friends, temp1, temp2) && !isWall (walls, temp1, temp2))
								{
									if (isFood (food, temp1, temp2))
										this->loseToughness (-1);
									next.mType = WALK;
									next.relativeX = temp1;
									next.relativeY = temp2;
									return next;
								}
							}
						}
						temp1 = enemies[i].relativeX - j + k;
						temp2 = enemies[i].relativeY + k;
						if (temp1 || temp2)
						{
							if (temp1 > -4 && temp2 < 4)
							{
								if (!isEnemy (enemies, temp1, temp2) && !isFriend (friends, temp1, temp2) && !isWall (walls, temp1, temp2))
								{
									if (isFood (food, temp1, temp2))
										this->loseToughness (-1);
									next.mType = WALK;
									next.relativeX = temp1;
									next.relativeY = temp2;
									return next;
								}
							}
						}
						temp1 = enemies[i].relativeX - j + k;
						temp2 = enemies[i].relativeY - k;
						if (temp1 || temp2)
						{
							if (temp1 > -4 && temp2 > -4)
							{
								if (!isEnemy (enemies, temp1, temp2) && !isFriend (friends, temp1, temp2) && !isWall (walls, temp1, temp2))
								{
									if (isFood (food, temp1, temp2))
										this->loseToughness (-1);
									next.mType = WALK;
									next.relativeX = temp1;
									next.relativeY = temp2;
									return next;
								}
							}
						}
					}
				}
			}
		}
		else
			break;
	}


	/*		Eating food		*/

	for (int i = 0; i < food.size(); ++i)
	{
		if (food[i].relativeX < 4 && food[i].relativeY < 4)
		{
			this->loseToughness (-1);
			next.mType = WALK;
			next.relativeX = food[i].relativeX;
			next.relativeY = food[i].relativeY;
			return next;
		}
	}


	/*		Move randomly		*/

	for (int i = -3; i < 4; ++i)
	{
		for (int j = -3; j < 4; ++j)
		{
			if ((i || j) && !isEnemy (enemies, i, j) && !isFriend (friends, i, j) && !isWall (walls, i, j))
			{
				next.mType = WALK;
				next.relativeX = i;
				next.relativeY = j;
				return next;
			}
		}
	}


	/*		No action		*/

	next.mType = NOACT;
	next.relativeX = 0;
	next.relativeY = 0;
	return next;
}


/*

			WIZARD
	Sight: 3
	Act: 3
	Actions: Freeze, Weaken, Walk, No Act
	Priority: Freeze > Weaken > Move closer to enemy > Move randomly > No act
	NOTE: No benefit from food
	Freeze - battlemage, wizard
	Weaken - others

*/


int Wizard :: getMana() const
{
	return this->mana;
}


void Wizard :: operator++()
{
	this->loseToughness (-3);
	this->mana += 2;
}


Move Wizard :: move (const vector <Object> &list)
{
	Move next;
	vector <Object> enemies, food, walls, friends;
	for (int i = 0; i < list.size(); ++i)
	{
		if (list[i].objType == FOOD)
			food.push_back (list[i]);
		else if (list[i].objType == WALL)
			walls.push_back (list[i]);
		else if (list[i].teamSide != this->getSide())
			enemies.push_back (list[i]);
		else if (list[i].teamSide == this->getSide())
			friends.push_back (list[i]);
	}

	sortUnits2 (enemies, 0, enemies.size() - 1);

	if (this->mana)
	{
		sortUnits2 (enemies, 0, enemies.size() - 1);

		/*		Freeze			*/

		for (int i = 0; i < enemies.size() && importance (enemies[i].objType) < 3; ++i)
		{
			if (abs (enemies[i].relativeX) < 4 && abs (enemies[i].relativeY) < 4)
			{
				--this->mana;
				next.mType = FREEZESPELL;
				next.relativeX = enemies[i].relativeX;
				next.relativeY = enemies[i].relativeY;
				return next;
			}
			else
				break;
		}


		/*		Weaken			*/

		for (int i = 0; i < enemies.size(); ++i)
		{
			if (abs (enemies[i].relativeX) < 4 && abs (enemies[i].relativeY) < 4)
			{
				--this->mana;
				next.mType = WEAKENSPELL;
				next.relativeX = enemies[i].relativeX;
				next.relativeY = enemies[i].relativeY;
				return next;
			}
			else
				break;
		}
	}


	/*		Moving closer to enemy		*/

	for (int i = 0; i < enemies.size(); ++i)
	{
		if (abs (enemies[i].relativeX) + abs (enemies[i].relativeY) < 7)
		{
			if (abs (enemies[i].relativeX) < 4 && abs (enemies[i].relativeY) < 4)
			{
				int dist = abs (enemies[i].relativeX) + abs (enemies[i].relativeY);
				for (int j = 1; j < dist; ++j)
				{
					for (int k = 0; k <= j; ++k)
					{
						int temp1 = enemies[i].relativeX + j - k, temp2 = enemies[i].relativeY + k;
						if (temp1 || temp2)
						{
							if (temp1 < 4 && temp2 < 4)
							{
								if (!isEnemy (enemies, temp1, temp2) && !isFriend (friends, temp1, temp2) && !isWall (walls, temp1, temp2))
								{
									next.mType = WALK;
									next.relativeX = temp1;
									next.relativeY = temp2;
									return next;
								}
							}
						}
						temp1 = enemies[i].relativeX + j - k;
						temp2 = enemies[i].relativeY - k;
						if (temp1 || temp2)
						{
							if (temp1 < 4 && temp2 > -4)
							{
								if (!isEnemy (enemies, temp1, temp2) && !isFriend (friends, temp1, temp2) && !isWall (walls, temp1, temp2))
								{
									next.mType = WALK;
									next.relativeX = temp1;
									next.relativeY = temp2;
									return next;
								}
							}
						}
						temp1 = enemies[i].relativeX - j + k;
						temp2 = enemies[i].relativeY + k;
						if (temp1 || temp2)
						{
							if (temp1 > -4 && temp2 < 4)
							{
								if (!isEnemy (enemies, temp1, temp2) && !isFriend (friends, temp1, temp2) && !isWall (walls, temp1, temp2))
								{
									next.mType = WALK;
									next.relativeX = temp1;
									next.relativeY = temp2;
									return next;
								}
							}
						}
						temp1 = enemies[i].relativeX - j + k;
						temp2 = enemies[i].relativeY - k;
						if (temp1 || temp2)
						{
							if (temp1 > -4 && temp2 > -4)
							{
								if (!isEnemy (enemies, temp1, temp2) && !isFriend (friends, temp1, temp2) && !isWall (walls, temp1, temp2))
								{
									next.mType = WALK;
									next.relativeX = temp1;
									next.relativeY = temp2;
									return next;
								}
							}
						}
					}
				}
			}
		}
		else
			break;
	}


	/*		Move randomly		*/

	for (int i = -3; i < 4; ++i)
	{
		for (int j = -3; j < 4; ++j)
		{
			if ((i || j) && !isEnemy (enemies, i, j) && !isFriend (friends, i, j) && !isWall (walls, i, j))
			{
				next.mType = WALK;
				next.relativeX = i;
				next.relativeY = j;
				return next;
			}
		}
	}


	/*		No action		*/

	next.mType = NOACT;
	next.relativeX = 0;
	next.relativeY = 0;
	return next;
}


/*

			CLERIC
	Sight: 2
	Act: 1
	Actions: Heal, Walk, No Act
	Priority: Heal > Move closer to friend > Move randomly > No act
	NOTE: No benefit from food
	No self-healing

*/


void Cleric :: operator++()
{
	this->loseToughness (-this->getActionPower());
}


Move Cleric :: move (const vector <Object> &list)
{
	Move next;
	vector <Object> enemies, food, walls, friends;
	for (int i = 0; i < list.size(); ++i)
	{
		if (list[i].objType == FOOD)
			food.push_back (list[i]);
		else if (list[i].objType == WALL)
			walls.push_back (list[i]);
		else if (list[i].teamSide != this->getSide())
			enemies.push_back (list[i]);
		else if (list[i].teamSide == this->getSide())
			friends.push_back (list[i]);
	}
	sortUnits (friends, 0, friends.size() - 1);


	/*		Healing friend			*/

	for (int i = 0; i < friends.size(); ++i)
	{
		if (abs (friends[i].relativeX) >= 0 && abs (friends[i].relativeX) < 2 && abs (friends[i].relativeY) >= 0 && abs (friends[i].relativeY) < 2)
		{
			next.mType = HEAL;
			next.relativeX = friends[i].relativeX;
			next.relativeY = friends[i].relativeY;
			return next;
		}
		else
			break;
	}

	sortUnits3 (friends, 0, friends.size() - 1);

	/*		Moving closer to friend		*/

	for (int i = 0; i < friends.size(); ++i)
	{
		if (abs (friends[i].relativeX) + abs (friends[i].relativeY) < 5)
		{
			if (abs (friends[i].relativeX) < 3 && abs (friends[i].relativeY) < 3)
			{
				if (friends[i].relativeX < 0)
				{
					if (friends[i].relativeY < 0)
					{
						if (!isEnemy (enemies, -1, -1) && !isWall (walls, -1, -1))
						{
							next.mType = WALK;
							next.relativeX = -1;
							next.relativeY = -1;
							return next;
						}
						else if (!isEnemy (enemies, -1, 0) && !isWall (walls, -1, 0))
						{
							next.mType = WALK;
							next.relativeX = -1;
							next.relativeY = 0;
							return next;
						}
						else if (!isEnemy (enemies, 0, -1) && !isWall (walls, 0, -1))
						{
							next.mType = WALK;
							next.relativeX = 0;
							next.relativeY = -1;
							return next;
						}
					}
					else if (friends[i].relativeY == 0)
					{
						if (!isEnemy (enemies, -1, 0) && !isWall (walls, -1, 0))
						{
							next.mType = WALK;
							next.relativeX = -1;
							next.relativeY = 0;
							return next;
						}
					}
					else
					{
						if (!isEnemy (enemies, -1, 1) && !isWall (walls, -1, 1))
						{
							next.mType = WALK;
							next.relativeX = -1;
							next.relativeY = 1;
							return next;
						}
						else if (!isEnemy (enemies, -1, 0) && !isWall (walls, -1, 0))
						{
							next.mType = WALK;
							next.relativeX = -1;
							next.relativeY = 0;
							return next;
						}
						else if (!isEnemy (enemies, 0, 1) && !isWall (walls, 0, 1))
						{
							next.mType = WALK;
							next.relativeX = 0;
							next.relativeY = 1;
							return next;
						}
					}
				}
				else if (friends[i].relativeX == 0)
				{
					if (friends[i].relativeY < 0)
					{
						if (!isEnemy (enemies, 0, -1) && !isWall (walls, 0, -1))
						{
							next.mType = WALK;
							next.relativeX = 0;
							next.relativeY = -1;
							return next;
						}
					}
					else
					{
						if (!isEnemy (enemies, 0, 1) && !isWall (walls, 0, 1))
						{
							next.mType = WALK;
							next.relativeX = 0;
							next.relativeY = 1;
							return next;
						}
					}
				}
				else
				{
					if (friends[i].relativeY < 0)
					{
						if (!isEnemy (enemies, 1, -1) && !isWall (walls, 1, -1))
						{
							next.mType = WALK;
							next.relativeX = 1;
							next.relativeY = -1;
							return next;
						}
						else if (!isEnemy (enemies, 1, 0) && !isWall (walls, 1, 0))
						{
							next.mType = WALK;
							next.relativeX = 1;
							next.relativeY = 0;
							return next;
						}
						else if (!isEnemy (enemies, 0, -1) && !isWall (walls, 0, -1))
						{
							next.mType = WALK;
							next.relativeX = 0;
							next.relativeY = -1;
							return next;
						}
					}
					else if (friends[i].relativeY == 0)
					{
						if (!isEnemy (enemies, 1, 0) && !isWall (walls, 1, 0))
						{
							next.mType = WALK;
							next.relativeX = 1;
							next.relativeY = 0;
							return next;
						}
					}
					else
					{
						if (!isEnemy (enemies, 1, 1) && !isWall (walls, 1, 1))
						{
							next.mType = WALK;
							next.relativeX = 1;
							next.relativeY = 1;
							return next;
						}
						else if (!isEnemy (enemies, 1, 0) && !isWall (walls, 1, 0))
						{
							next.mType = WALK;
							next.relativeX = 1;
							next.relativeY = 0;
							return next;
						}
						else if (!isEnemy (enemies, 0, 1) && !isWall (walls, 0, 1))
						{
							next.mType = WALK;
							next.relativeX = 0;
							next.relativeY = 1;
							return next;
						}
					}
				}
			}
		}
		else
			break;
	}


	/*		Moving randomly			*/

	if (!isEnemy (enemies, -1, -1) && !isWall (walls, -1, -1))
	{
		next.mType = WALK;
		next.relativeX = -1;
		next.relativeY = -1;
		return next;
	}
	else if (!isEnemy (enemies, -1, 0) && !isWall (walls, -1, 0))
	{
		next.mType = WALK;
		next.relativeX = -1;
		next.relativeY = 0;
		return next;
	}
	else if (!isEnemy (enemies, 0, -1) && !isWall (walls, 0, -1))
	{
		next.mType = WALK;
		next.relativeX = 0;
		next.relativeY = -1;
		return next;
	}
	else if (!isEnemy (enemies, 1, -1) && !isWall (walls, 1, -1))
	{
		next.mType = WALK;
		next.relativeX = 1;
		next.relativeY = -1;
		return next;
	}
	else if (!isEnemy (enemies, 1, 0) && !isWall (walls, 1, 0))
	{
		next.mType = WALK;
		next.relativeX = 1;
		next.relativeY = 0;
		return next;
	}
	else if (!isEnemy (enemies, 1, 1) && !isWall (walls, 1, 1))
	{
		next.mType = WALK;
		next.relativeX = 1;
		next.relativeY = 1;
		return next;
	}
	else if (!isEnemy (enemies, 0, 1) && !isWall (walls, 0, 1))
	{
		next.mType = WALK;
		next.relativeX = 0;
		next.relativeY = 1;
		return next;
	}
	else if (!isEnemy (enemies, -1, 1) && !isWall (walls, -1, 1))
	{
		next.mType = WALK;
		next.relativeX = -1;
		next.relativeY = 1;
		return next;
	}


	/*		No action		*/

	next.mType = NOACT;
	next.relativeX = 0;
	next.relativeY = 0;
	return next;
}


/*

			BATTLEMAGE
	Sight: 3
	Act: 3
	Actions: Pro Heal, Freeze, Weaken, Walk, No Act
	Priority: Self-healing > Freeze > Weaken > Heal > Move closer to friend > Move randomly > No act
	NOTE: No benefit from food
	Freeze - battlemage, wizard
	Weaken - others
	Self-healing if HP is below 3 (not 3)

*/


void BattleMage :: operator++()
{
	this->loseToughness (-this->getActionPower());
	this->loseMana (-1);
}


Move BattleMage :: move (const vector <Object> &list)
{
	Move next;
	if (this->getToughness() < 3)
	{
		next.mType = HEAL;
		next.relativeX = 0;
		next.relativeY = 0;
		return next;
	}
	vector <Object> enemies, food, walls, friends;
	for (int i = 0; i < list.size(); ++i)
	{
		if (list[i].objType == FOOD)
			food.push_back (list[i]);
		else if (list[i].objType == WALL)
			walls.push_back (list[i]);
		else if (list[i].teamSide != this->getSide())
			enemies.push_back (list[i]);
		else if (list[i].teamSide == this->getSide())
			friends.push_back (list[i]);
	}


	/*		Behaving like wizard		*/

	if (this->getMana())
	{
		sortUnits2 (enemies, 0, enemies.size() - 1);

		/*		Freeze			*/

		for (int i = 0; i < enemies.size() && importance (enemies[i].objType) < 3; ++i)
		{
			if (abs (enemies[i].relativeX) < 4 && abs (enemies[i].relativeY) < 4)
			{
				this->loseMana(1);
				next.mType = FREEZESPELL;
				next.relativeX = enemies[i].relativeX;
				next.relativeY = enemies[i].relativeY;
				return next;
			}
			else
				break;
		}


		/*		Weaken			*/

		for (int i = 0; i < enemies.size(); ++i)
		{
			if (abs (enemies[i].relativeX) < 4 && abs (enemies[i].relativeY) < 4)
			{
				this->loseMana(1);
				next.mType = WEAKENSPELL;
				next.relativeX = enemies[i].relativeX;
				next.relativeY = enemies[i].relativeY;
				return next;
			}
			else
				break;
		}
	}


	/*		Behaving like cleric		*/

	sortUnits (friends, 0, friends.size() - 1);

	for (int i = 0; i < friends.size(); ++i)
	{
		if (abs (friends[i].relativeX) < 4 && abs (friends[i].relativeY) < 4)
		{
			next.mType = HEAL;
			next.relativeX = friends[i].relativeX;
			next.relativeY = friends[i].relativeY;
			return next;
		}
		else
			break;
	}


	/*		Move randomly		*/

	for (int i = -3; i < 4; ++i)
	{
		for (int j = -3; j < 4; ++j)
		{
			if ((i || j) && !isEnemy (enemies, i, j) && !isFriend (friends, i, j) && !isWall (walls, i, j))
			{
				next.mType = WALK;
				next.relativeX = i;
				next.relativeY = j;
				return next;
			}
		}
	}


	/*		No action		*/

	next.mType = NOACT;
	next.relativeX = 0;
	next.relativeY = 0;
	return next;
}
