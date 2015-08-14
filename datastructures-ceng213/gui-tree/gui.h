#ifndef __GUI_H__
#define __GUI_H__
#include <iostream>
#include <string>
#include <vector>
#include "component.h"

using namespace std;

class GUI
{
public:
	/**
	 * Constructs a GUI object with the given @size (dimensions).
	 * 
	 * @size[in] an instance of std::pair<int,int> that specifies the
	 *           dimensions of the graphical user interface (GUI)
	 */
	GUI(Size size);
	/**
	 * @return the root Component of the Component tree (hierarchy)
	 */
	const Component *getRoot() const;
	Component*                  getRoot();
	/**
	 * @return the size/dimensions of the root Component
	 */
	Size                        getSize();
	/**
	 * This method inserts a new component to the GUI. The @component 
	 * is inserted under the deepest component that covers the upper 
	 * left corner of the argument @component. If there are more than 
	 * one such component that satisfy this criteria, choose one 
	 * arbitrarily. 
	 * 
	 * @component[in] a pointer to a Component object
	 */
	void                        insertComponent(Component* component);
	/**
	 * This method removes the component called @name and all its children 
	 * recursively. Note: "root" component will never be removed. 
	 * 
	 * @name[in] the name of the component
	 * @return   true if the component exists (and removed), false otherwise
	 */
	bool                        removeComponent(string name);
	/**
	 * This method returns the deepest component that contains the given @point. 
	 * In case there are more than one such component return one of them 
	 * arbitrarily. 
	 * 
	 * @point[in] an instance of std::pair<int,int> that specifies a point
	 * @return    a pointer to target Component
	 */
	Component*                  contains(Position point);
	/**
	 * This method computes cumulative area usage of each component in the GUI. 
	 * Since each component is a rectangle, the area is computed using their 
	 * width and height. In addition, each component sums up the area usage of 
	 * its children recursively. This is similar to the "du" command in Linux. 
	 * Components will be inserted to the vector in post-order.
	 *
	 * return a vector containing component name, total area pairs
	 */
	vector< pair<string,int> >  areaUsage();
	/**
	 * This method returns a matrix that displays the components as a 2-D scene. 
	 * This matrix contains positive numbers at a location P if P contains an edge. 
	 * Note that components that are at lower levels (i.e., deeper in the 
	 * hierarchy) occlude components that are at higher levels. 
	 * 
	 * @return a matrix as a two-dimensional jagged array
	 */
	int**                       toArray();
	/**
	 * This overloaded operator<< prints the GUI component tree in a specific 
	 * format in which each level is printed on a separate line and children of 
	 * each node is enclosed with '|' characters. For instance, the following tree, 
	 *     a
	 *   /   \
	 *  b     c
	 * is printed as:
	 * | a | 
	 * | b c | 
	 * | | | |
	 * Also, the following tree,
	 *      a
	 *   /\   \
	 *  b  c   d
	 *         |
	 *         e
	 * is printed as:
	 * | a | 
	 * | b c d |
	 * | | | | | e |
	 * | |
	 */
	friend ostream&             operator<<(ostream& os, const GUI& gui);
	friend void fillByBreadth (vector <Component *> *result, Component *root, int counter);
	friend Component *searchByName (string name, Component *root);
	friend void fillByArea (vector < pair <string, int> > *pairs, Component *root);
	friend void addArea (Component *node, int toAdd);
	friend void subtractArea (Component *node, int toSubtr);
	// TODO: add methods if required
private:
	Component *root;
	// TODO: add fields/methods if required
};

#endif
