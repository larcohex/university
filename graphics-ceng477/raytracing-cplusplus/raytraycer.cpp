#include <iostream>
#include <vector>
#include <cmath>
#include "parseScene.h"
#include "writePPM.h"
#include "figures.h"


using namespace std;


float checkTriangle (Vertex &D, Vertex &camera, Triangle &triangle)
{
	float a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, denom, t;
	a = triangle.vertices[0].x - triangle.vertices[1].x;
	b = triangle.vertices[0].x - triangle.vertices[2].x;
	c = D.x;
	d = triangle.vertices[0].x - camera.x;

	e = triangle.vertices[0].y - triangle.vertices[1].y;
	f = triangle.vertices[0].y - triangle.vertices[2].y;
	g = D.y;
	h = triangle.vertices[0].y - camera.y;

	i = triangle.vertices[0].z - triangle.vertices[1].z;
	j = triangle.vertices[0].z - triangle.vertices[2].z;
	k = D.z;
	l = triangle.vertices[0].z - camera.z;

	m = f * k - g * j;
	n = h * k - g * l;
	o = f * l - h * j;
	p = g * i - e * k;
	q = e * j - f * i;
	denom = a * m + b * p + c * q;
	float beta = (d * m - b * n - c * o) / denom;
	if (beta < 0.0)
	{
		t = -1;
		return t;
	}
	r = e * l - h * i;
	float gamma = (a * n + d * p + c * r) / denom;
	if (gamma < 0.0)
	{
		t = -1;
		return t;
	}
	if (beta + gamma > 1.0)
	{
		t = -1;
		return t;
	}
	t = (a * o - b * r + d * q) / denom;
	if (t < 0.0000001)
	{
		t = -1;
	}
	return t;
}


float checkSphere (Vertex d, Sphere &sphere)
{
	float t;
	float a, b, c, disc;
	a = d.x * d.x + d.y * d.y + d.z * d.z;;
	b = 2.0 * (d.x * sphere.o_c.x + d.y * sphere.o_c.y + d.z * sphere.o_c.z);
	c = (sphere.o_c.x * sphere.o_c.x + sphere.o_c.y * sphere.o_c.y + sphere.o_c.z * sphere.o_c.z) - sphere.radius * sphere.radius;
	disc = b * b - 4.0 * a * c;
	if (disc >= 0.0)
	{
		float t_1 = (-b - sqrt (disc)) / (2.0 * a);
		float t_2 = (-b + sqrt (disc)) / (2.0 * a);
		if (t_1 > 0.0000001)
		{
			t = t_1;
		}
		else if (t_2 > 0.0000001)
		{
			t = t_2;
		}
		else
		{
			t = -1;
		}
	}
	else
	{
		t = -1;
	}
	return t;
}


RGB rayTracing (Vertex ray, Vertex a, vector <Triangle> &triangles, vector <Sphere> &spheres, vector <PointLight> &pointLights, RGB &ambientLight, RGB &backgroundColor, int rayReflectionCount)
{
	RGB resultRGB;
	Vertex temp;
	float minT = -1;
	int figType = 0;
	int figID = -1;
	for (int j = 0; j < spheres.size(); ++j)
	{
		Vertex o_c;
		o_c.x = a.x - spheres[j].center.x;
		o_c.y = a.y - spheres[j].center.y;
		o_c.z = a.z - spheres[j].center.z;
		spheres[j].o_c = o_c;
	}
	for (int l = 0; l < triangles.size(); ++l)
	{
		float temp = checkTriangle (ray, a, triangles[l]);
		if (temp != -1)
		{
			if (minT == -1 || temp < minT)
			{
				minT = temp;
				figType = 1;
				figID = triangles[l].id;
			}
		}	
	}
	for (int l = 0; l < spheres.size(); ++l)
	{
		// TODO: check if intersects, if so, compare with nearest object
		float temp = checkSphere (ray, spheres[l]);
		if (temp != - 1)
		{
			if (minT == -1 || temp < minT)
			{
				minT = temp;
				figType = 2;
				figID = spheres[l].id;
			}
		}
	}
	if (minT == -1)
	{
		resultRGB.r = backgroundColor.r;
		resultRGB.b = backgroundColor.b;
		resultRGB.b = backgroundColor.b;
		return resultRGB;
	}
	else
	{
		RGB reflectance;
		Vertex r;
		Vertex interPoint;
		if (figType == 1)
		{
			Triangle tr;
			for (int m = 0; m < triangles.size(); ++m)
			{
				if (triangles[m].id == figID)
				{
					tr = triangles[m];
					break;
				}
			}
			reflectance.r = tr.material.reflectance.r;
			reflectance.g = tr.material.reflectance.g;
			reflectance.b = tr.material.reflectance.b;
			resultRGB.r = tr.material.ambient.r * ambientLight.r;
			resultRGB.g = tr.material.ambient.g * ambientLight.g;
			resultRGB.b = tr.material.ambient.b * ambientLight.b;
			interPoint.x = a.x + minT * ray.x;
			interPoint.y = a.y + minT * ray.y;
			interPoint.z = a.z + minT * ray.z;
			Vertex v;
			temp.x = a.x - interPoint.x;
			temp.y = a.y - interPoint.y;
			temp.z = a.z - interPoint.z;
			v.x = temp.x / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
			v.x = (v.x > -0.0000001 && v.x <= 0) ? 0 : v.x;
			v.y = temp.y / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
			v.y = (v.y > -0.0000001 && v.y <= 0) ? 0 : v.y;
			v.z = temp.z / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
			v.z = (v.z > -0.0000001 && v.z <= 0) ? 0 : v.z;
			for (int m = 0; m < pointLights.size(); ++m)
			{
				Vertex l;
				temp.x = pointLights[m].position.x - interPoint.x;
				temp.y = pointLights[m].position.y - interPoint.y;
				temp.z = pointLights[m].position.z - interPoint.z;
				float distSq = temp.x * temp.x + temp.y * temp.y + temp.z * temp.z;
				l.x = temp.x / sqrt (distSq);
				l.x = (l.x > -0.0000001 && l.x <= 0) ? 0 : l.x;
				l.y = temp.y / sqrt (distSq);
				l.y = (l.y > -0.0000001 && l.y <= 0) ? 0 : l.y;
				l.z = temp.z / sqrt (distSq);
				l.z = (l.z > -0.0000001 && l.z <= 0) ? 0 : l.z;
				bool intersects = false;
				for (int n = 0; n < triangles.size(); ++n)
				{
					if (triangles[n].id == tr.id)
					{
						continue;
					}
					float q = checkTriangle (temp, interPoint, triangles[n]);
					if (q != -1)
					{
						intersects = true;
						break;
					}
				}
				for (int n = 0; n < spheres.size(); ++n)
				{
					spheres[n].o_c.x = interPoint.x - spheres[n].center.x;
					spheres[n].o_c.y = interPoint.y - spheres[n].center.y;
					spheres[n].o_c.z = interPoint.z - spheres[n].center.z;
					float q = checkSphere (temp, spheres[n]);
					spheres[n].o_c.x = a.x - spheres[n].center.x;
					spheres[n].o_c.y = a.y - spheres[n].center.y;
					spheres[n].o_c.z = a.z - spheres[n].center.z;
					if (q != -1)
					{
						intersects = true;
						break;
					}
				}
				if (!intersects)
				{
					RGB intensity;
					intensity.r = pointLights[m].intensity.r / (4.0 * 3.14 * distSq);
					intensity.g = pointLights[m].intensity.g / (4.0 * 3.14 * distSq);
					intensity.b = pointLights[m].intensity.b / (4.0 * 3.14 * distSq);
					float nl = l.x * tr.normal.x + l.y * tr.normal.y + l.z * tr.normal.z;
					Vertex h;
					temp.x = l.x + v.x;
					temp.y = l.y + v.y;
					temp.z = l.z + v.z;
					h.x = temp.x / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
					h.x = (h.x > -0.0000001 && h.x <= 0) ? 0 : h.x;
					h.y = temp.y / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
					h.y = (h.y > -0.0000001 && h.y <= 0) ? 0 : h.y;
					h.z = temp.z / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
					h.z = (h.z > -0.0000001 && h.z <= 0) ? 0 : h.z;
					float nh = h.x * tr.normal.x + h.y * tr.normal.y + h.z * tr.normal.z;
					resultRGB.r += tr.material.diffuse.r * intensity.r * max (float (0), nl) 
							+ tr.material.specular.r * intensity.r * pow (max (float (0), nh), tr.material.specExp);
					resultRGB.g += tr.material.diffuse.g * intensity.g * max (float (0), nl) 
							+ tr.material.specular.g * intensity.g * pow (max (float (0), nh), tr.material.specExp);
					resultRGB.b += tr.material.diffuse.b * intensity.b * max (float (0), nl) 
							+ tr.material.specular.b * intensity.b * pow (max (float (0), nh), tr.material.specExp);
				}
			}
			float nv = v.x * tr.normal.x + v.y * tr.normal.y + v.z * tr.normal.z;
			temp.x = 2 * nv * tr.normal.x - v.x;
			temp.y = 2 * nv * tr.normal.y - v.y;
			temp.z = 2 * nv * tr.normal.z - v.z;
			r.x = temp.x / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
			r.x = (r.x > -0.0000001 && r.x <= 0) ? 0 : r.x;
			r.y = temp.y / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
			r.y = (r.y > -0.0000001 && r.y <= 0) ? 0 : r.y;
			r.z = temp.z / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
			r.z = (r.z > -0.0000001 && r.z <= 0) ? 0 : r.z;
			interPoint.x += temp.x * 0.0075;
			interPoint.y += temp.y * 0.0075;
			interPoint.z += temp.z * 0.0075;
		}
		else
		{
			Sphere sp;
			for (int m = 0; m < spheres.size(); ++m)
			{
				if (spheres[m].id == figID)
				{
					sp = spheres[m];
					break;
				}
			}
			reflectance.r = sp.material.reflectance.r;
			reflectance.g = sp.material.reflectance.g;
			reflectance.b = sp.material.reflectance.b;
			resultRGB.r = sp.material.ambient.r * ambientLight.r;
			resultRGB.g = sp.material.ambient.g * ambientLight.g;
			resultRGB.b = sp.material.ambient.b * ambientLight.b;
			interPoint.x = a.x + minT * ray.x;
			interPoint.y = a.y + minT * ray.y;
			interPoint.z = a.z + minT * ray.z;
			sp.normal.x = (interPoint.x - sp.center.x) / sp.radius;
			sp.normal.y = (interPoint.y - sp.center.y) / sp.radius;
			sp.normal.z = (interPoint.z - sp.center.z) / sp.radius;
			Vertex v;
			temp.x = a.x - interPoint.x;
			temp.y = a.y - interPoint.y;
			temp.z = a.z - interPoint.z;
			v.x = temp.x / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
			v.x = (v.x > -0.0000001 && v.x <= 0) ? 0 : v.x;
			v.y = temp.y / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
			v.y = (v.y > -0.0000001 && v.y <= 0) ? 0 : v.y;
			v.z = temp.z / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
			v.z = (v.z > -0.0000001 && v.z <= 0) ? 0 : v.z;
			for (int m = 0; m < pointLights.size(); ++m)
			{
				Vertex l;
				Vertex temp;
				temp.x = pointLights[m].position.x - interPoint.x;
				temp.y = pointLights[m].position.y - interPoint.y;
				temp.z = pointLights[m].position.z - interPoint.z;
				float distSq = temp.x * temp.x + temp.y * temp.y + temp.z * temp.z;
				l.x = temp.x / sqrt (distSq);
				l.x = (l.x > -0.0000001 && l.x <= 0) ? 0 : l.x;
				l.y = temp.y / sqrt (distSq);
				l.y = (l.y > -0.0000001 && l.y <= 0) ? 0 : l.y;
				l.z = temp.z / sqrt (distSq);
				l.z = (l.z > -0.0000001 && l.z <= 0) ? 0 : l.z;
				bool intersects = false;
				for (int n = 0; n < triangles.size(); ++n)
				{
					float d = checkTriangle (temp, interPoint, triangles[n]);
					if (d != -1)
					{
						intersects = true;
						break;
					}
				}
				for (int n = 0; n < spheres.size(); ++n)
				{
					if (spheres[n].id == sp.id)
					{
						continue;
					}
					spheres[n].o_c.x = interPoint.x - spheres[n].center.x;
					spheres[n].o_c.y = interPoint.y - spheres[n].center.y;
					spheres[n].o_c.z = interPoint.z - spheres[n].center.z;
					float d = checkSphere (temp, spheres[n]);
					spheres[n].o_c.x = a.x - spheres[n].center.x;
					spheres[n].o_c.y = a.y - spheres[n].center.y;
					spheres[n].o_c.z = a.z - spheres[n].center.z;
					if (d != -1)
					{
						intersects = true;
						break;
					}
				}
				if (!intersects)
				{
					RGB intensity;
					intensity.r = pointLights[m].intensity.r / (4.0 * 3.14 * distSq);
					intensity.g = pointLights[m].intensity.g / (4.0 * 3.14 * distSq);
					intensity.b = pointLights[m].intensity.b / (4.0 * 3.14 * distSq);
					float nl = l.x * sp.normal.x + l.y * sp.normal.y + l.z * sp.normal.z;

					Vertex h;
					temp.x = l.x + v.x;
					temp.y = l.y + v.y;
					temp.z = l.z + v.z;
					h.x = temp.x / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
					h.x = (h.x > -0.0000001 && h.x <= 0) ? 0 : h.x;
					h.y = temp.y / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
					h.y = (h.y > -0.0000001 && h.y <= 0) ? 0 : h.y;
					h.z = temp.z / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
					h.z = (h.z > -0.0000001 && h.z <= 0) ? 0 : h.z;
					float nh = h.x * sp.normal.x + h.y * sp.normal.y + h.z * sp.normal.z;
					resultRGB.r += sp.material.diffuse.r * intensity.r * max (float (0), nl) 
							+ sp.material.specular.r * intensity.r * pow (max (float (0), nh), sp.material.specExp);
					resultRGB.g += sp.material.diffuse.g * intensity.g * max (float (0), nl) 
							+ sp.material.specular.g * intensity.g * pow (max (float (0), nh), sp.material.specExp);
					resultRGB.b += sp.material.diffuse.b * intensity.b * max (float (0), nl) 
							+ sp.material.specular.b * intensity.b * pow (max (float (0), nh), sp.material.specExp);
				}
			}
			float nv = v.x * sp.normal.x + v.y * sp.normal.y + v.z * sp.normal.z;
			temp.x = 2 * nv * sp.normal.x - v.x;
			temp.y = 2 * nv * sp.normal.y - v.y;
			temp.z = 2 * nv * sp.normal.z - v.z;
			r.x = temp.x / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
			r.x = (r.x > -0.0000001 && r.x <= 0) ? 0 : r.x;
			r.y = temp.y / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
			r.y = (r.y > -0.0000001 && r.y <= 0) ? 0 : r.y;
			r.z = temp.z / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
			r.z = (r.z > -0.0000001 && r.z <= 0) ? 0 : r.z;
			interPoint.x += temp.x * 0.0075;
			interPoint.y += temp.y * 0.0075;
			interPoint.z += temp.z * 0.0075;
		}
		if (rayReflectionCount && (reflectance.r != 0 || reflectance.g != 0 || reflectance.b != 0))
		{
			return resultRGB + (reflectance * rayTracing (r, interPoint, triangles, spheres, pointLights, ambientLight, backgroundColor, rayReflectionCount - 1));
		}
		else
		{
			return resultRGB;
		}
	}
}



int main(int argc, char* argv[])
{
	//
	// Test XML parsing
	//
	vector <Camera> cameras;
	vector <Material> materials;
	vector <Vertex> vertices;
	vector <Triangle> triangles;
	vector <Sphere> spheres;
	vector <PointLight> pointLights;
	RGB ambientLight, backgroundColor;
	int rayReflectionCount;
    
	bool result = parseSceneXML(argv[1], materials, vertices, triangles, spheres, pointLights, ambientLight, backgroundColor, rayReflectionCount, cameras);

//	Connecting with materials and vertices



	for (int i = 0; i < triangles.size(); ++i)
	{
		bool found1, found2, found3;
		found1 = found2 = found3 = false;
		for (int j = 0; j < vertices.size(); ++j)
		{
			if (found1 && found2 && found3)
			{
				break;
			}
			if (!found1 && vertices[j].id == triangles[i].vIds[0])
			{
				triangles[i].vertices[0] = vertices[j];
				found1 = true;
				continue;
			}
			if (!found2 && vertices[j].id == triangles[i].vIds[1])
			{
				triangles[i].vertices[1] = vertices[j];
				found2 = true;
				continue;
			}
			if (!found3 && vertices[j].id == triangles[i].vIds[2])
			{
				triangles[i].vertices[2] = vertices[j];
				found3 = true;
				continue;
			}
		}
		for (int j = 0; j < materials.size(); ++j)
		{
			if (materials[j].id == triangles[i].mId)
			{
				triangles[i].material = materials[j];
				break;
			}
		}
		Vertex c_a, b_a;
		c_a.x = triangles[i].vertices[1].x - triangles[i].vertices[0].x;
		c_a.y = triangles[i].vertices[1].y - triangles[i].vertices[0].y;
		c_a.z = triangles[i].vertices[1].z - triangles[i].vertices[0].z;
		b_a.x = triangles[i].vertices[2].x - triangles[i].vertices[0].x;
		b_a.y = triangles[i].vertices[2].y - triangles[i].vertices[0].y;
		b_a.z = triangles[i].vertices[2].z - triangles[i].vertices[0].z;
		Vertex c_axb_a;
		c_axb_a.x = c_a.y * b_a.z - c_a.z * b_a.y;
		c_axb_a.y = c_a.z * b_a.x - c_a.x * b_a.z;
		c_axb_a.z = c_a.x * b_a.y - c_a.y * b_a.x;
		float value = sqrt (c_axb_a.x * c_axb_a.x + c_axb_a.y * c_axb_a.y + c_axb_a.z * c_axb_a.z);
		Vertex normal;
		normal.x = c_axb_a.x / value;
		normal.y = c_axb_a.y / value;
		normal.z = c_axb_a.z / value;
		triangles[i].normal = normal;
	}



	for (int i = 0; i < spheres.size(); ++i)
	{
		for (int j = 0; j < vertices.size(); ++j)
		{
			if (vertices[j].id == spheres[i].cId)
			{
				spheres[i].center = vertices[j];
				break;
			}
		}
		for (int j = 0; j < materials.size(); ++j)
		{
			if (materials[j].id == spheres[i].mId)
			{
				spheres[i].material = materials[j];
				break;
			}
		}
	}


//	Creating image plane



	for (int i = 0; i < cameras.size(); ++i)
	{
		vector <float> resultPlane;
		Vertex u;
		Vertex temp;
		temp.x = cameras[i].gaze.y * cameras[i].up.z - cameras[i].gaze.z * cameras[i].up.y;
		temp.x = (temp.x > -0.0000001 && temp.x <= 0) ? 0 : temp.x;
		temp.y = cameras[i].gaze.z * cameras[i].up.x - cameras[i].gaze.x * cameras[i].up.z;
		temp.y = (temp.y > -0.0000001 && temp.y <= 0) ? 0 : temp.y;
		temp.z = cameras[i].gaze.x * cameras[i].up.y - cameras[i].gaze.y * cameras[i].up.x;
		temp.z = (temp.z > -0.0000001 && temp.z <= 0) ? 0 : temp.z;
		u.x = temp.x / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
		u.x = (u.x > -0.0000001 && u.x <= 0) ? 0 : u.x;
		u.y = temp.y / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
		u.y = (u.y > -0.0000001 && u.y <= 0) ? 0 : u.y;
		u.z = temp.z / sqrt (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z);
		u.z = (u.z > -0.0000001 && u.z <= 0) ? 0 : u.z;
		cameras[i].u = u;
		for (int j = 0; j < cameras[i].imagePlane.horRes; ++j)
		{
			Vertex d, sU_U, Wd;
			float sU = cameras[i].imagePlane.left + (cameras[i].imagePlane.right - cameras[i].imagePlane.left) * (j + 0.5) / cameras[i].imagePlane.horRes;
			sU_U.x = sU * cameras[i].u.x;
			sU_U.y = sU * cameras[i].u.y;
			sU_U.z = sU * cameras[i].u.z;
			Wd.x = cameras[i].imagePlane.distance * cameras[i].gaze.x;
			Wd.y = cameras[i].imagePlane.distance * cameras[i].gaze.y;
			Wd.z = cameras[i].imagePlane.distance * cameras[i].gaze.z;
			for (int k = 0; k < cameras[i].imagePlane.verRes; ++k)
			{
				float sV = cameras[i].imagePlane.bottom + (cameras[i].imagePlane.top - cameras[i].imagePlane.bottom) * (k + 0.5) / cameras[i].imagePlane.verRes;
				Vertex sV_V;
				sV_V.x = sV * cameras[i].up.x;
				sV_V.y = sV * cameras[i].up.y;
				sV_V.z = sV * cameras[i].up.z;
				d.x = sU_U.x + sV_V.x + Wd.x;
				d.y = sU_U.y + sV_V.y + Wd.y;
				d.z = sU_U.z + sV_V.z + Wd.z;
				RGB resultRGB = rayTracing (d, cameras[i].position, triangles, spheres, pointLights, ambientLight, backgroundColor, rayReflectionCount);
				resultRGB.r = (resultRGB.r > 255.0) ? 255.0 : resultRGB.r;
				resultRGB.g = (resultRGB.g > 255.0) ? 255.0 : resultRGB.g;
				resultRGB.b = (resultRGB.b > 255.0) ? 255.0 : resultRGB.b;
				resultPlane.push_back (resultRGB.r);
				resultPlane.push_back (resultRGB.g);
				resultPlane.push_back (resultRGB.b);
			}
		}
		float *result = &resultPlane[0];
		writePPM (cameras[i].outputName.c_str(), cameras[i].imagePlane.horRes, cameras[i].imagePlane.verRes, result);
	}

	return 0;
}
