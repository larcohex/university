#include <iostream>
#include <cstdio>
#include "tinyXML/tinyxml.h"
#include "figures.h"

bool parseSceneXML(const char* filename, vector <Material> &materials, vector <Vertex> &vertices, vector <Triangle> &triangles, vector <Sphere> &spheres, vector <PointLight> &pointLights, RGB &ambientLight, RGB &backgroundColor, int &rayReflectionCount, vector <Camera> &cameras)
{
	TiXmlDocument doc(filename);
	bool loadOkay = doc.LoadFile();

	if (!loadOkay)
	{
		std::cout << "Could not load file: " << filename << "Error = " << doc.ErrorDesc() << std::endl;
		return false;
	}

	TiXmlNode* pRoot = doc.FirstChild("Scene");
	for (TiXmlNode* pNode = pRoot->FirstChild(); pNode; pNode = pNode->NextSibling())
	{
        if (pNode->Value() == std::string("Material"))
        {
			TiXmlAttribute* pAtt = pNode->ToElement()->FirstAttribute();
			int index = pAtt->IntValue(); // get material index

            //
            // read reflectance coefficients
            //
            float amb[3], dif[3], spe[3], mir[3];
			float phongExp;
			for (TiXmlNode* pChild = pNode->FirstChild(); pChild; pChild = pChild->NextSibling())
			{
				if (pChild->Value() == std::string("Ambient"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &amb[0], &amb[1], &amb[2]);
				}
				else if (pChild->Value() == std::string("Diffuse"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &dif[0], &dif[1], &dif[2]);
				}
				else if (pChild->Value() == std::string("Specular"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f %f", &spe[0], &spe[1], &spe[2], &phongExp);
				}
				else if (pChild->Value() == std::string("Reflectance"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &mir[0], &mir[1], &mir[2]);
				}
			}

            //
            // TODO: Save the scanned values for the current material in your data structures.
            //
			RGB newAmb, newDif, newSpe, newMir;
			newAmb.r = amb[0];
			newAmb.g = amb[1];
			newAmb.b = amb[2];
			newDif.r = dif[0];
			newDif.g = dif[1];
			newDif.b = dif[2];
			newSpe.r = spe[0];
			newSpe.g = spe[1];
			newSpe.b = spe[2];
			newMir.r = mir[0];
			newMir.g = mir[1];
			newMir.b = mir[2];
			Material newMaterial;
			newMaterial.id = index;
			newMaterial.ambient = newAmb;
			newMaterial.diffuse = newDif;
			newMaterial.specular = newSpe;
			newMaterial.reflectance = newMir;
			newMaterial.specExp = phongExp;
			materials.push_back (newMaterial);
        }
		else if (pNode->Value() == std::string("Vertex"))
        {
			TiXmlAttribute* pAtt = pNode->ToElement()->FirstAttribute();
			int index = pAtt->IntValue(); // get vertex index

			float coords[3];
			TiXmlNode* pChild = pNode->FirstChild();
			sscanf(pChild->FirstChild()->Value(), "%f %f %f", &coords[0], &coords[1], &coords[2]);

            //
            // TODO: Save the scanned values for the current vertex in your data structures.
            //
			Vertex newVertex;
			newVertex.id = index;
			newVertex.x = coords[0];
			newVertex.y = coords[1];
			newVertex.z = coords[2];
			vertices.push_back (newVertex);
		}
		else if (pNode->Value() == std::string("Triangle"))
        {
			TiXmlAttribute* pAtt = pNode->ToElement()->FirstAttribute();
			int index = pAtt->IntValue(); // get triangle index

			int vIndex[3], mIndex;
			for (TiXmlNode* pChild = pNode->FirstChild(); pChild; pChild = pChild->NextSibling())
			{
				if (pChild->Value() == std::string("Vertices"))
				{
					sscanf(pChild->FirstChild()->Value(), "%d %d %d", &vIndex[0], &vIndex[1], &vIndex[2]);
				}
				else if (pChild->Value() == std::string("MaterialId"))
				{
					sscanf(pChild->FirstChild()->Value(), "%d", &mIndex);
				}
			}

            //
            // TODO: Save the scanned values for the current triangle in your data structures.
            //
			Triangle newTriangle;
			newTriangle.id = index;
			newTriangle.vIds[0] = vIndex[0];
			newTriangle.vIds[1] = vIndex[1];
			newTriangle.vIds[2] = vIndex[2];
			newTriangle.mId = mIndex;
			triangles.push_back (newTriangle);
		}
		else if (pNode->Value() == std::string("Sphere"))
        {
			TiXmlAttribute* pAtt = pNode->ToElement()->FirstAttribute();
			int index = pAtt->IntValue(); // Sphere index

			int vIndex, mIndex;
			float rad;
			for (TiXmlNode* pChild = pNode->FirstChild(); pChild; pChild = pChild->NextSibling())
			{
				if (pChild->Value() == std::string("Center"))
				{
					sscanf(pChild->FirstChild()->Value(), "%d", &vIndex);
				}
				else if (pChild->Value() == std::string("MaterialId"))
				{
					sscanf(pChild->FirstChild()->Value(), "%d", &mIndex);
				}
				else if (pChild->Value() == std::string("Radius"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f", &rad);
				}
			}

            //
            // TODO: Save the scanned values for the current sphere in your data structures.
            //
			Sphere newSphere;
			newSphere.id = index;
			newSphere.cId = vIndex;
			newSphere.mId = mIndex;
			newSphere.radius = rad;
			spheres.push_back (newSphere);
		}
		else if (pNode->Value() == std::string("PointLight"))
        {
			TiXmlAttribute* pAtt = pNode->ToElement()->FirstAttribute();
			int index = pAtt->IntValue(); // Light index

			float pos[3], intensity[3];
			for (TiXmlNode* pChild = pNode->FirstChild(); pChild; pChild = pChild->NextSibling())
			{
				if (pChild->Value() == std::string("Position"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &pos[0], &pos[1], &pos[2]);
				}
				else if (pChild->Value() == std::string("Intensity"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &intensity[0], &intensity[1], &intensity[2]);
				}
			}

            //
            // TODO: Save the scanned values for the current point light in your data structures.
            //
			PointLight newPointLight;
			Vertex newVertex;
			newVertex.x = pos[0];
			newVertex.y = pos[1];
			newVertex.z = pos[2];
			RGB newRGB;
			newRGB.r = intensity[0];
			newRGB.g = intensity[1];
			newRGB.b = intensity[2];
			newPointLight.id = index;
			newPointLight.position = newVertex;
			newPointLight.intensity = newRGB;
			pointLights.push_back (newPointLight);
		}
		else if (pNode->Value() == std::string("AmbientLight"))
        {
			float intensity[3];
			TiXmlNode* pChild = pNode->FirstChild();
			sscanf(pChild->Value(), "%f %f %f", &intensity[0], &intensity[1], &intensity[2]);

            //
            // TODO: Save the scanned values for the ambient light in your data structures.
            //
			ambientLight.r = intensity[0];
			ambientLight.g = intensity[1];
			ambientLight.b = intensity[2];
		}
		else if (pNode->Value() == std::string("BackgroundColor"))
        {
            float bgColor[3];
			TiXmlNode* pChild = pNode->FirstChild();
			sscanf(pChild->Value(), "%f %f %f", &bgColor[0], &bgColor[1], &bgColor[2]);

            //
            // TODO: Save the scanned values for the background color in your data structures.
            //
			backgroundColor.r = bgColor[0];
			backgroundColor.g = bgColor[1];
			backgroundColor.b = bgColor[2];
		}
		else if (pNode->Value() == std::string("RayReflectionCount"))
        {
            int rayReflectCount;
			TiXmlNode* pChild = pNode->FirstChild();
			sscanf(pChild->Value(), "%d", &rayReflectCount);

            //
            // TODO: Save the scanned values for the ray reflection count in your data structures.
            //
			rayReflectionCount = rayReflectCount;
		}
		else if (pNode->Value() == std::string("Camera"))
        {
			TiXmlAttribute* pAtt = pNode->ToElement()->FirstAttribute();
			int index = pAtt->IntValue(); // Camera index
			float gaze[3], up[3], pos[3];
			float left, right, bottom, top, distance;
			int nx, ny;
			std::string imageName;
			for (TiXmlNode* pChild = pNode->FirstChild(); pChild; pChild = pChild->NextSibling())
			{
				if (pChild->Value() == std::string("Position"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &pos[0], &pos[1], &pos[2]);
				}
				else if (pChild->Value() == std::string("Gaze"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &gaze[0], &gaze[1], &gaze[2]);
				}
				else if (pChild->Value() == std::string("Up"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &up[0], &up[1], &up[2]);
				}
				else if (pChild->Value() == std::string("ImagePlane"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f %f %f %d %d", &left, &right, &bottom, &top, &distance, &nx, &ny);
				}
				else if (pChild->Value() == std::string("OutputName"))
				{
					imageName = pChild->FirstChild()->Value();
				}
			}

            //
            // TODO: Save the scanned values for the current camera in your data structures.
            //

			Camera newCamera;
			Vertex newPosition, newGaze, newUp;
			newPosition.x = pos[0];
			newPosition.y = pos[1];
			newPosition.z = pos[2];
			newGaze.x = gaze[0];
			newGaze.y = gaze[1];
			newGaze.z = gaze[2];
			newUp.x = up[0];
			newUp.y = up[1];
			newUp.z = up[2];
			Plane newPlane;
			newPlane.left = left;
			newPlane.right = right;
			newPlane.top = top;
			newPlane.bottom = bottom;
			newPlane.distance = distance;
			newPlane.horRes = nx;
			newPlane.verRes = ny;
			newCamera.id = index;
			newCamera.position = newPosition;
			newCamera.gaze = newGaze;
			newCamera.up = newUp;
			newCamera.imagePlane = newPlane;
			newCamera.outputName = imageName;
			cameras.push_back (newCamera);
		}
	}

    return true;
}
