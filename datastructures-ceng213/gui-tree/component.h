#ifndef __COMPONENT_H__
#define __COMPONENT_H__
#include <string>
#include <vector>

using namespace std;

typedef pair<int,int> Position;
typedef pair<int,int> Size;

class Component
{
public:
	/**
	 * Constructs a Component object using given parameters. Initially, 
	 * a component has no child child components and its parent is set 
	 * to NULL. 
	 *
	 * @name[in]     a string that specifies the name of the component
	 * @size[in]     std::pair<int,int> that specifies the size/dimensions 
	 *               of the component
	 * @position[in] std::pair<int,int> that specifies the position of 
	 *               the component
	 */
	Component(string name, Size size, Position position);
	/**
	 * Gets/sets the name of the component. 
	 */
	string               getName();
	void                 setName(string name);
	/**
	 * Gets/sets the size/dimensions of the component. 
	 */
	Size                 getSize();
	void                 setSize(Size size);
	/**
	 * Gets/sets the positions of the component. 
	 */
	Position             getPosition();
	void                 setPosition(Position pos);
	/**
	 * Gets/sets the parent of the component. If this is the root 
	 * component, "getParent" returns NULL.
	 */
	Component*           getParent();
	void                 setParent(Component* component);
	/**
	 * Adds a new component as a child of this component. This method 
	 * does not check whether the child component is enclosed by 
	 * this component. This is the job of the GUI::insertComponent. 
	 */
	void                 addChild(Component* child);
	/**
	 * Returns a pointer to the component having the name specified 
	 * by the argument. 
	 */
	Component*           getChild(string name);
	/**
	 * Removes the component having the name specified by the argument.
	 * It does not delete/free the removed component; it only removes the 
	 * specified component from the children list. 
	 *
	 * @name  the name of the component to be removed
	 * return true if the component exists and removed, false otherwise
	 */
	bool                 removeChild(string name);
	/**
	 * Returns all child components as a vector. Note: it only returns 
	 * its children, not grand-children or lower descendents. 
	 *
	 * return a reference to a vector of Component pointers
	 */
	vector<Component*>&  getChildren();
	int getAreaUsage();
	void setAreaUsage(int newArea);
	// TODO: add methods if required
private:
	string name;
	Size size;
	Position position;
	Component *parent;
	vector <Component *> children;
	int areaUsage;
	// TODO: add methods/fields if required
};
#endif
