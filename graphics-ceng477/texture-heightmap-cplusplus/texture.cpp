#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <GL/gl.h>   
#include <GL/glut.h>
#include "SOIL.h"
#include "glinit.h"
#include "utils.h"
#include "glentry.h"
#include <pthread.h>

using namespace std;

struct vertex
{
	float x;
	float y;
	float z;
};


struct uv
{
	float u;
	float v;
};

struct Sphere
{
	int latitude;
	int longitude;
	float radius;
	vector <vertex> vertices;
	vector <vertex> normvs;
	vector <uv> uvs;
	unsigned int vVBO;
	unsigned int nVBO;
	unsigned int uvVBO;
};



Sphere earth;
unsigned int earthTexture;
unsigned int heightTexture;
unsigned int trVBO;
vector <unsigned int> trIndices;
GLuint gProgramCubeMap;
GLuint gTextureCubeMap;
GLuint gVerticesVBO, gIndicesVBO;
GLuint gEarthVerticesVBO, gEarthIndicesVBO;
GLfloat gDispLevel = 0.1;
GLuint gEarthRender;


float gCamAngle = 0.0f;
float gCamDirX = 0.0f;
float gCamDirZ = 0.0f;
float gCamDist = 5.0f;


float gEarthAngle = 0.0f;
float gEarthAngleSun = 0.0f;
float gEarthPosX = 0.0f;
float gEarthPosZ = -1.0f;


void bugFix()
{
	int i;
	i = pthread_getconcurrency();
};


void reshape (int w, int h) 
{
	w = w < 1 ? 1 : w;
	h = h < 1 ? 1 : h;

	glViewport (0, 0, w, h);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (45, double(w)/h, 0.1f, 1000);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}
		
void mouse (int button, int state, int x, int y)
{
	if (button == 3 && state == GLUT_UP)
	{
		gCamDist -= 0.1f;
	}
	else if (button == 4 && state == GLUT_UP)
	gCamDist += 0.1f;
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key) 
	{
		case 27:
			exit(0); 
			break; 
		case 'a' :
			gCamAngle -= 0.01f;
			break;
		case 'd' :
			gCamAngle += 0.01f;
			break;
		case '-':
			gDispLevel -= 0.01f;
			break;
		case '+':
			gDispLevel += 0.01f;
			break;
		default:
			break;
	}
}

void init (int *argc, char** argv) 
{
	glEnable (GL_DEPTH_TEST);
	glEnableClientState (GL_VERTEX_ARRAY);
	glShadeModel (GL_SMOOTH);
	initTextures();
	initShaders();
	if (*argc < 3) {
		cout << "Missing argument(s)." << endl 
			 << "Usage: ./texture <# of Latitudes> <# of Longitudes> <radius of the Earth>" << endl;
			 
		exit(0);	
	}
	initCubeMap (&gVerticesVBO, &gIndicesVBO);
	earth.latitude = atoi (argv[*argc - 3]);
	earth.longitude = atoi (argv[*argc - 2]);
	earth.radius = atof (argv[*argc - 1]);
	glUniform1i (glGetUniformLocation (gEarthRender, "earthTexture") , 0);
	glUniform1i (glGetUniformLocation (gEarthRender, "heightTexture") , 1);
	for (int i = 0; i < earth.latitude + 1; ++i)
	{
		for (int j = 0; j < earth.longitude + 1; ++j)
		{
			vertex newVertex;
			uv newUV;
			newVertex.x = cos (3.14159265 * ((float)(i - 1) / earth.latitude - 0.5)) * cos (2 * 3.14159265 * (float)(j - 1) / earth.longitude) * earth.radius;
			newVertex.y = cos (3.14159265 * ((float)(i - 1) / earth.latitude - 0.5)) * sin (2 * 3.14159265 * (float)(j - 1) / earth.longitude) * earth.radius;
			newVertex.z = sin (3.14159265 * ((float)(i - 1) / earth.latitude - 0.5)) * earth.radius;
			earth.vertices.push_back (newVertex);
			newVertex.x = newVertex.x / (newVertex.x * newVertex.x + newVertex.y * newVertex.y + newVertex.z * newVertex.z);
			newVertex.y = newVertex.y / (newVertex.x * newVertex.x + newVertex.y * newVertex.y + newVertex.z * newVertex.z);
			newVertex.z = newVertex.z / (newVertex.x * newVertex.x + newVertex.y * newVertex.y + newVertex.z * newVertex.z);
			newUV.u = 1 - i / (earth.latitude + 2.0);
			newUV.v = 1 - j / (earth.longitude + 1.0);
			earth.normvs.push_back (newVertex);
			earth.uvs.push_back (newUV);
		}
	}
	vector <float> vBuffer, nBuffer, uvBuffer;
	for (int i = 0; i < earth.vertices.size(); ++i)
	{
		vBuffer.push_back (earth.vertices[i].x);
		vBuffer.push_back (earth.vertices[i].y);
		vBuffer.push_back (earth.vertices[i].z);
	}
	for (int i = 0; i < earth.normvs.size(); ++i)
	{
		nBuffer.push_back (earth.normvs[i].x);
		nBuffer.push_back (earth.normvs[i].y);
		nBuffer.push_back (earth.normvs[i].z);
	}
	for (int i = 0; i < earth.uvs.size(); ++i)
	{
		uvBuffer.push_back (earth.uvs[i].u);
		uvBuffer.push_back (earth.uvs[i].v);
	}
	for (int i = 0; i < earth.latitude; ++i)
	{
		for (int j = 0; j < earth.longitude; ++j)
		{
			trIndices.push_back (i * (earth.longitude + 1) + j);
			trIndices.push_back (i * (earth.longitude + 1) + j + 1);
			trIndices.push_back ((i + 1) * (earth.longitude + 1) + j);
			trIndices.push_back (i * (earth.longitude + 1) + j + 1);
			trIndices.push_back ((i + 1) * (earth.longitude + 1) + j + 1);
			trIndices.push_back ((i + 1) * (earth.longitude + 1) + j);
		}
	}
	glGenBuffers(1, &(earth.vVBO));
	glGenBuffers(1, &(earth.nVBO));
	glGenBuffers(1, &(earth.uvVBO));
	glGenBuffers(1, &(trVBO));
	
	glBindBuffer(GL_ARRAY_BUFFER, earth.vVBO);
	glBufferData(GL_ARRAY_BUFFER, vBuffer.size() * sizeof (GLfloat), vBuffer.data(), GL_STATIC_DRAW );
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, earth.nVBO);
	glBufferData(GL_ARRAY_BUFFER, nBuffer.size() * sizeof (GLfloat), nBuffer.data(), GL_STATIC_DRAW );
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, earth.uvVBO);
	glBufferData(GL_ARRAY_BUFFER, uvBuffer.size() * sizeof (GLfloat), uvBuffer.data(), GL_STATIC_DRAW );
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, trIndices.size() * sizeof (GLuint), trIndices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void display()
{	
	long startTime = getCurrentTime();

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	
	glLoadIdentity();
	

	gEarthAngleSun += 0.002739f;
	gEarthPosX = sin (gEarthAngleSun) * 50;
	gEarthPosZ = cos (gEarthAngleSun) * 50;

	gCamDirX = sin (gCamAngle);
	gCamDirZ = cos (gCamAngle);

	gluLookAt (gEarthPosX + gCamDirX*gCamDist, 0, gEarthPosZ + gCamDirZ*gCamDist, gEarthPosX, 0, gEarthPosZ, 0.0f, 1, 0.0f);

	GLfloat sunpos[4] = {0.0, 0.0 , 0.0,1};
	glLightfv (GL_LIGHT0, GL_POSITION, sunpos);
	GLfloat sunDiff[3] = {1, 1, 1};
	GLfloat sunAmb[3] = {0.25, 0.25, 0.25};

	glLightfv (GL_LIGHT0, GL_POSITION, sunpos);
	glLightfv (GL_LIGHT0, GL_DIFFUSE, sunDiff);
	glLightfv (GL_LIGHT0, GL_AMBIENT, sunAmb);



	// Stars -- Cubemap
	glPushMatrix();
		glScalef (300,100,300);
		glUseProgram (gProgramCubeMap);
		
		glBindBuffer (GL_ARRAY_BUFFER, gVerticesVBO);
		glVertexPointer (3, GL_FLOAT, sizeof(float) * 3, 0);
		
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, gIndicesVBO);
		glDrawElements (GL_QUADS, 24, GL_UNSIGNED_INT,  0);
	glPopMatrix();
		
	// Earth
	glPushMatrix();
		glUseProgram (gEarthRender);
		gEarthAngle += 1;
		glTranslatef (gEarthPosX, 0, gEarthPosZ);
		glRotatef (23.5f, 0, 0, 1);
		glRotatef (gEarthAngle, 0, 1, 0);
		
		// TODO: Display the Earth	
		glUniform1i (glGetUniformLocation (gEarthRender, "earthTexture"), 0);
		glUniform1f (glGetUniformLocation (gEarthRender, "hScale"), gDispLevel);
		glActiveTexture (earthTexture);
		glBindTexture (GL_TEXTURE_2D, GL_TEXTURE0);
		glActiveTexture (heightTexture);
		glBindTexture (GL_TEXTURE_2D, GL_TEXTURE1);
		glBindBuffer (GL_ARRAY_BUFFER, earth.vVBO);
		glVertexPointer (3, GL_FLOAT, sizeof(float) * 3, 0);
		glEnableClientState (GL_NORMAL_ARRAY);
		glBindBuffer (GL_ARRAY_BUFFER, earth.nVBO);
		glNormalPointer (GL_FLOAT, sizeof(float) * 3, 0);
		glEnableClientState (GL_TEXTURE_COORD_ARRAY);
		glBindBuffer (GL_ARRAY_BUFFER, earth.uvVBO);
		glTexCoordPointer (2, GL_FLOAT, sizeof(float) * 2, 0);
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, trVBO);
		glDrawElements (GL_TRIANGLES, trIndices.size(), GL_UNSIGNED_INT,  0);
	glPopMatrix();
	// Sun
	glPushMatrix();
		glUseProgram(0); // 0 means using no program		
		glColor3f(1,1,0);
		glutSolidSphere(1, 20, 20);
	glPopMatrix();
	
	glutSwapBuffers();
	// Set FPS to 100
	long diff = getCurrentTime() - startTime;
	if (diff < 10000)
	{
		usleep(10000 - diff);
	}
}

int main (int argc, char** argv)   
{	
	// Hack. Handle the error OpenGL - NVidia consistency error.
	bugFix();
	
	glutInit (&argc, argv);

	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowPosition (100, 100);
	glutInitWindowSize (800, 800);
	glutCreateWindow ("Earth model");

	init_glentry();
	init (&argc, argv);
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);
	glutMouseFunc (mouse);
	glutKeyboardFunc (keyboard);
	glutIdleFunc (glutPostRedisplay);

	glutMainLoop();

	return 0;
}
