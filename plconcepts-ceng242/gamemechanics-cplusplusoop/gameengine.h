#ifndef _GAMEENGINE_H_
#define _GAMEENGINE_H_
#include <vector>

#include "mechanics.h"

struct Coordinate
{
 	   int locationX;
 	   int locationY;
};

struct CreatureInfo
{
 	   Coordinate coor;
 	   int type;
	   int power;
	   int toughness;
	   int mana;
	   int fireCount;
};

class GameEngine {
  private:
	std::vector<CreatureInfo> northTeamInfo; 
    std::vector<CreatureInfo> southTeamInfo; 
    int sizeOfMaze, numberOfTurns, sizeOfTeam, numberOfFood;
	
	std::vector<Creature*> northTeam;
	std::vector<Creature*> southTeam;
	std::vector<Coordinate> foodList;
	void readInput();
	void createTeams(std::vector<Creature*>& team, const std::vector<CreatureInfo>& teamInfo, Side s);
	bool oneTurn();
	
  public:
	GameEngine();
	~GameEngine();
	void simulateGame();
};

#endif
