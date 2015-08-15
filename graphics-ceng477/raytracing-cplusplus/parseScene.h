#ifndef __parseScene_h__
#define __parseScene_h__
#include <vector>
#include "figures.h"
using namespace std;

bool parseSceneXML(const char* filename, vector <Material> &materials, vector <Vertex> &vertices, vector <Triangle> &triangles, vector <Sphere> &spheres, vector <PointLight> &pointLights, RGB &ambientLight, RGB &backgroundColor, int &rayReflectionCount, vector <Camera> &cameras);

#endif //__parseScene_h__
