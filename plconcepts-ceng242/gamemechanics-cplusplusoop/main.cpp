#include "mechanics.h"
#include <vector>
#include <iostream>


using namespace std;

void sortUnits (vector <Object> &v, int begin, int end);

int main()
{
	Object cl1, cl2, dw1, dw2;
	cl1.objType = CLERIC;
	cl2.objType = CLERIC;
	dw1.objType = DWARF;
	dw2.objType = DWARF;
	cl1.teamSide = SOUTH;
	cl2.teamSide = SOUTH;
	dw1.teamSide = SOUTH;
	dw2.teamSide = SOUTH;
	cl1.bewitched = false;
	cl2.bewitched = false;
	dw1.bewitched = false;
	dw2.bewitched = false;
	cl1.relativeX = -2;
	cl1.relativeY = 1;
	cl2.relativeX = 2;
	cl2.relativeY = 1;
	dw1.relativeX = -1;
	dw1.relativeY = 1;
	dw2.relativeX = -1;
	dw2.relativeY = 2;
	vector <Object> test;
	test.push_back (cl1);
	test.push_back (cl2);
	test.push_back (dw1);
	test.push_back (dw2);
	sortUnits (test, 0, test.size() - 1);
	cout << "\n\n\n\n\n\n\n" << endl;
	for (int i = 0; i < test.size(); ++i)
	{
		cout << test[i].relativeX << " " << test[i].relativeY << endl;
	}
	return 0;
}
