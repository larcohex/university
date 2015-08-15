#ifndef FIGURES_H
#define FIGURES_H


#include <vector>
#include <string>


using namespace std;


struct RGB
{
	float r;
	float g;
	float b;
};


struct Plane
{
	float left;
	float right;
	float bottom;
	float top;
	float distance;
	int horRes;
	int verRes;
};



struct Material
{
	int id;
	RGB ambient;
	RGB diffuse;
	RGB specular;
	RGB reflectance;
	float specExp;
};


struct Vertex
{
	int id;
	float x;
	float y;
	float z;
};


struct Triangle
{
	int id;
	int vIds[3];
	int mId;
	Vertex vertices[3];
	Vertex normal;
	Material material;
};


struct Sphere
{
	int id;
	int cId;
	float radius;
	int mId;
	Vertex center;
	Material material;
	Vertex o_c;
	Vertex normal;
};


struct PointLight
{
	int id;
	Vertex position;
	RGB intensity;
};


struct Camera
{
	int id;
	Vertex position;
	Vertex gaze;
	Vertex up;
	Vertex u;
	Plane imagePlane;
	string outputName;		
};


inline RGB operator* (const RGB &rgb1, const RGB &rgb2)
{
	RGB result;
	result.r = rgb1.r * rgb2.r;
	result.g = rgb1.g * rgb2.g;
	result.b = rgb1.b * rgb2.b;
	return result;
}


inline RGB operator+ (const RGB &rgb1, const RGB &rgb2)
{
	RGB result;
	result.r = rgb1.r + rgb2.r;
	result.g = rgb1.g + rgb2.g;
	result.b = rgb1.b + rgb2.b;
	return result;
}



#endif
