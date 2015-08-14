#include "gameengine.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

ostream& operator <<(ostream& os , const Creature& c) 
{ 
         os<< "Creature Side: " << (c.getSide() ? "SOUTH" : "NORTH") << " Action Power: "<< c.getActionPower() << " Toughness: "<< c.getToughness() << " Bewitched: " << c.getBewitched();
}

ostream& operator <<(ostream& os , const Wizard& c) 
{ 
         os<< "Creature Side: " << (c.getSide() ? "SOUTH" : "NORTH") << " Action Power: "<< c.getActionPower() << " Toughness: "<< c.getToughness() << " Bewitched: " << c.getBewitched() << " Mana: " << c.getMana();
}

ostream& operator <<(ostream& os , const Dragon& c) 
{ 
         os<< "Creature Side: " << (c.getSide() ? "SOUTH" : "NORTH") << " Action Power: "<< c.getActionPower() << " Toughness: "<< c.getToughness() << " Bewitched: " << c.getBewitched() << " Fire Count: " << c.getLeftFireCount();
}

void GameEngine::readInput() {
	ifstream inpf("hw5.inp", ios::in);
	if (!inpf.is_open()) {
		cout << "Could not open hw5.inp" << endl;
		exit(1);
	}
	inpf >> sizeOfMaze;
	inpf >> numberOfTurns;
	inpf >> sizeOfTeam;
	inpf >> numberOfFood;

	for (int i=0 ; i<numberOfFood ; i++) 
	{ 
		Coordinate fcoor;
		inpf >> fcoor.locationX >> fcoor.locationY;
		
		foodList.push_back(fcoor);
	}
	
	for (int i=0 ; i<sizeOfTeam ; i++) 
	{ //left top corner of the maze is (0,0)
		CreatureInfo teamMember;
		inpf >> teamMember.coor.locationX >> teamMember.coor.locationY >> teamMember.type >> teamMember.power >> teamMember.toughness;
		teamMember.mana = -1;
		teamMember.fireCount = -1;
		if (teamMember.type == 0 || teamMember.type == 1)
			inpf >> teamMember.mana;
		else if(teamMember.type == 5)
			inpf >> teamMember.fireCount;
		
		northTeamInfo.push_back(teamMember);
	}
	
	for (int i=0 ; i<sizeOfTeam ; i++) 
	{
		CreatureInfo teamMember;
		inpf >> teamMember.coor.locationX >> teamMember.coor.locationY >> teamMember.type >> teamMember.power >> teamMember.toughness;
		teamMember.mana = -1;
		teamMember.fireCount = -1;
		if (teamMember.type == 0 || teamMember.type == 1)
			inpf >> teamMember.mana;
		else if(teamMember.type == 5)
			inpf >> teamMember.fireCount;
		
		southTeamInfo.push_back(teamMember);
	}
	
	inpf.close();
}

void GameEngine::createTeams(vector<Creature*>& team, const vector<CreatureInfo>& teamInfo, Side s)
{
	for(int i = 0; i < teamInfo.size(); i++)
		{
			switch(teamInfo[i].type) {
				case 0: 
					team.push_back(new BattleMage(s, teamInfo[i].power, teamInfo[i].toughness, teamInfo[i].mana));	
					break;
				case 1: 
					team.push_back(new Wizard(s, teamInfo[i].power, teamInfo[i].toughness, teamInfo[i].mana));	
					break;
				case 2: 
					team.push_back(new Cleric(s, teamInfo[i].power, teamInfo[i].toughness));	
					break;
				case 3: 
					team.push_back(new Human(s, teamInfo[i].power, teamInfo[i].toughness));	
					break;
				case 4: 
					team.push_back(new Dwarf(s, teamInfo[i].power, teamInfo[i].toughness));
					break;
				case 5: 
					team.push_back(new Dragon(s, teamInfo[i].power, teamInfo[i].toughness, teamInfo[i].fireCount));	
					break;
				default: 
					cerr<<"invalid type of warrior"<<endl;
			}
		}
}

GameEngine::GameEngine() {
	
    readInput();
	
	createTeams(northTeam, northTeamInfo, NORTH);
	createTeams(southTeam, southTeamInfo, SOUTH);

}

GameEngine::~GameEngine()
{
}

bool GameEngine::oneTurn() {
	
	Move action;
	vector<Object> objectList;
	cout<<sizeOfTeam<<endl;
	for (int i=0 ; i<sizeOfTeam ; i++) {
		// Here sight range of the creature will be examined and result will be assigned to objectList
		// This part is not complete, you can complete to test your code
		action = northTeam[i]->move(objectList);
		
		cout<<*northTeam[i]<<endl;
		
		/* //Instead of printing just creature information, you can print more detailed information as shown below. 
           // Note that Wizard, BattlaMage, and Dragon information can also be reached without dynamic casting, since creature type is stores in teamInfo vectors.
        if(dynamic_cast<Wizard*>(northTeam[i])!= NULL) cout<<*(dynamic_cast<Wizard*>(northTeam[i]))<<endl;
		else if(dynamic_cast<Dragon*>(northTeam[i])!= NULL) cout<<*(dynamic_cast<Dragon*>(northTeam[i]))<<endl;
		else cout<<*northTeam[i]<<endl;*/
		
		// Here action will be examined and necessary actions will be done accordingly, 
		// For instance; if a dragon returns a DRAGONPAW action for a human enemy, 
		// the toughness of the human will be decreased by calling loseToughness function of the human, and if the toughness becomes 0 or below, 
		// the human will be destructed.
		// This part is not complete, you can complete to test your code

		// Here sight range of player will be examined and result will be assigned to objectList
		// This part is not complete, you can complete to test your code
		action = southTeam[i]->move(objectList);
		
		cout<<*southTeam[i]<<endl;
			
		/*  //Instead of printing just creature information, you can print more detailed information as shown below. 
            //Note that Wizard, BattlaMage, and Dragon information can also be reached without dynamic casting, since creature type is stores in teamInfo vectors.
        if(dynamic_cast<Wizard*>(southTeam[i])!= NULL) cout<<*(dynamic_cast<Wizard*>(southTeam[i]))<<endl;
		else if(dynamic_cast<Dragon*>(southTeam[i])!= NULL) cout<<*(dynamic_cast<Dragon*>(southTeam[i]))<<endl;
		else cout<<*southTeam[i]<<endl; */
		
		// Here action will be examined and necessary actions will be done accordingly,
		// For instance; if a cleric returns a HEAL action for a human friend, 
		// the toughness of the human will be increased by calling the ++ operator of the human.
		// This part is not complete, you can complete to test your code
		
	}
	// finish the Game
	// This part is not complete, you can complete to test your code
	
	return true;
	//Check whether there is an illegal move. If it is illegal, return false;
}

void GameEngine::simulateGame() {
	for (int i=0 ; i<numberOfTurns ; i++) {
		cout << "Turn is now: " << i << endl;
		if (!oneTurn()) return;
	}
}

