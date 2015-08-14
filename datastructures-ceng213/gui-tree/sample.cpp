#include "component.h"
#include "gui.h"
#include <iostream>

using namespace std;

/**
 * This function prints the area usage for each component 
 * using the GUI::areaUsage() method.
 */
void printAU(ostream& os, GUI& gui)
{
	os << "----------" << endl;
	vector< pair<string,int> > areas = gui.areaUsage();
	for (int i=0; i < areas.size(); ++i) {
		pair<string,int> node = areas[i];
		os << areas[i].first << " " << areas[i].second << endl;
	}
}

/**
 * This function prints the GUI as a two dimensional grid using the 
 * GUI::toArray() method. It prints a star character ('*') for each 
 * positive number of the array. 
 */
void print2D(ostream& os, GUI& gui)
{
	int** array = gui.toArray();
	Size size = gui.getSize();
	for (int i=0; i < size.first; i++) {
		for (int j=0; j < size.second; j++) {
			if (array[i][j] == 0) {
				os << ' ';
			} else {
				os << '*';
			}
		}
		os << endl;
	}
}

/* Sample code block - 1 */
int main1()
{
	GUI gui(Size(10,10));
	Component* c;
	c = new Component("panel1", Size(4,5), Position(2,2));
	gui.insertComponent(c);
	printAU(cout, gui);  // Area-usage
	cout << gui;         // Hierarchical form
	cout << "----------" << endl;
	print2D(cout, gui);  // 2-D scene form
	return 0;
}
/* Sample code block - 2 */
int main2()
{
	GUI gui(Size(30,30));
	Component* c;
	c = new Component("panel1", Size(10,10), Position(5,5));
	gui.insertComponent(c);
	c = new Component("panel2", Size(10,10), Position(15,15));
	gui.insertComponent(c);
	c = new Component("button1", Size(5,5), Position(7,7));
	gui.insertComponent(c);
	printAU(cout, gui);  // Area-usage
	cout << gui;         // Hierarchical form
	cout << "----------" << endl;
	print2D(cout, gui);  // 2-D scene form
	return 0;
}
/* Sample code block - 3 */
int main3()
{
	GUI gui(Size(30,30));
	Component* c;
	c = new Component("panel1", Size(20,20), Position(5,5));
	gui.insertComponent(c);
	c = new Component("button1", Size(10,5), Position(10,10));
	gui.insertComponent(c);
	c = new Component("label1", Size(10,10), Position(20,20));
	gui.insertComponent(c);
	c = new Component("panel2", Size(4,8), Position(20,0));
	gui.insertComponent(c);
	c = new Component("image1", Size(5,10), Position(12,15));
	gui.insertComponent(c);
	printAU(cout, gui);  // Area-usage
	cout << gui;         // Hierarchical form
	cout << "----------" << endl;
	print2D(cout, gui);  // 2-D scene form
	return 0;
}
/* Sample code block - 4 */
int main4()
{
	GUI gui(Size(80,80));
	Component* c;
	c = new Component("panel1", Size(20,20), Position(20,20));
	gui.insertComponent(c);
	c = new Component("button1", Size(10,5), Position(25,25));
	gui.insertComponent(c);
	c = new Component("label1", Size(30,30), Position(40,40));
	gui.insertComponent(c);
	c = new Component("panel2", Size(60,10), Position(50,50));
	gui.insertComponent(c);
	c = new Component("button2", Size(5,5), Position(22,22));
	gui.insertComponent(c);
	c = new Component("radio1", Size(2,2), Position(23,23));
	gui.insertComponent(c);
	c = new Component("panel3", Size(40,40), Position(70,70));
	gui.insertComponent(c);
	printAU(cout, gui);  // Area-usage
	cout << gui;         // Hierarchical form
	cout << "----------" << endl;
	print2D(cout, gui);  // 2-D scene form
	return 0;
}
/* Main entry point of the application */
int main(int argc, char*argv[])
{
	char select = '1';
	if (argc == 2) {
		select = argv[1][0];
	}
	switch (select) {
	case '1':
		return main1();
		break;
	case '2':
		return main2();
	case '3':
		return main3();
	case '4':
		return main4();
	default:
		return main1();
	}
	return 0;
}
