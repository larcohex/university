#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>


using namespace std;


struct vertex
{
	float x;
	float y;
	float z;
};


struct RGB
{
	float r;
	float g;
	float b;
};


int main_window;


bool TM = true;
vertex surfaceTranslate;
float surfaceRotate = 0;
vertex camera;
vertex up;
int currentPointX = 0, currentPointY = 0;
vertex toY;

int SampleR, SampleC;
int M, N;
int K;
vector < vector <vertex> > points;
vector <RGB> palette;
vector < vector <vertex> > control;
float minH, maxH;


int fact (int n)
{
	if (!n || n == 1)
	{
		return 1;
	}
	return n * fact (n - 1);
}

int C (int n, int i)
{
	return fact (n) / (fact (i) * fact (n - i));
}


int min (int n, int m)
{
	if (n <= m)
	{
		return n;
	}
	return m;
}


void initialDisplay(void)
{
	glClearColor (1.0, 1.0, 1.0, 1.0);
	glDisable (GL_LIGHTING);
	glEnable (GL_RESCALE_NORMAL);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (60, 1, 1, 1000000);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}


void updateControl()
{
	control.clear();
	for (int i = 0; i < SampleR; ++i)
	{
		vector <vertex> temp;
		for (int j = 0; j < SampleC; ++j)
		{
			float u = (float)(i) / (SampleR - 1);
			float v = (float)(j) / (SampleC - 1);
			vertex p;
			p.x = p.y = p.z = 0;
			for (int k = 0; k < M; ++k)
			{
				for (int m = 0; m < N; ++m)
				{
					float B_u = C (M - 1, k) * pow (u, k) * pow (1 - u, M - 1 - k);
					float B_v = C (N - 1, m) * pow (v, m) * pow (1 - v, N - 1 - m);
					p.x += B_u * B_v * points[k][m].x;
					p.y += B_u * B_v * points[k][m].y;
					p.z += B_u * B_v * points[k][m].z;
				}
			}
			temp.push_back (p);
		}
		control.push_back (temp);
	}
	maxH = 1 << ((sizeof(float) * 8) - 1);
	minH = -maxH;
	for (int i = 0; i < SampleR - 1; ++i)
	{
		for (int j = 0; j < SampleC - 1; ++j)
		{
			float h = (control[i][j].y + control[i + 1][j].y + control[i][j + 1].y) / 3;
			if (h > maxH)
			{
				maxH = h;
			}
			if (h < minH)
			{
				minH = h;
			}
			h = (control[i + 1][j].y + control[i][j + 1].y + control[i + 1][j + 1].y) / 3;
			if (h > maxH)
			{
				maxH = h;
			}
			if (h < minH)
			{
				minH = h;
			}
		}
	}
	for (int i = 0; i < SampleR; ++i)
	{
		for (int j = 0; j < SampleC; ++j)
		{
			toY.x += control[i][j].x;
			toY.z += control[i][j].z;
		}
		toY.x /= SampleR * SampleC;
		toY.z /= SampleR * SampleC;
	}
}


void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt (camera.x, camera.y, camera.z, 0, 0, 0, up.x, up.y, up.z);
	glTranslatef (surfaceTranslate.x, surfaceTranslate.y, surfaceTranslate.z);
	glTranslatef (toY.x, toY.y, toY.z);
	glRotatef (surfaceRotate, 0, 1, 0);
	glTranslatef (- toY.x, -toY.y, -toY.z);
	if (!TM)
	{
		glPointSize (10);
		glBegin (GL_POINTS);
		for (int i = 0; i < M; ++i)
		{
			for (int j = 0; j < N; ++j)
			{
				if (i == currentPointX && j == currentPointY)
				{
					glColor3f (1.0, 0.0, 0.0);
				}
				else
				{
					glColor3f (0.0, 0.0, 1.0);
				}
				glVertex3f (points[i][j].x, points[i][j].y, points[i][j].z);
			}
		}
		glEnd();
	}
	glBegin (GL_TRIANGLES);
	for (int i = 0; i < SampleR - 1; ++i)
	{
		for (int j = 0; j < SampleC - 1; ++j)
		{
			float h = (control[i][j].y + control[i + 1][j].y + control[i][j + 1].y) / 3;
			int index = min (K - 1, K * (h - minH) / (maxH - minH));
			glColor3f (palette[index].r / 255, palette[index].g / 255, palette[index].b / 255);
			glVertex3f (control[i][j].x, control[i][j].y, control[i][j].z);
			glVertex3f (control[i + 1][j].x, control[i + 1][j].y, control[i + 1][j].z);
			glVertex3f (control[i][j + 1].x, control[i][j + 1].y, control[i][j + 1].z);
			h = (control[i + 1][j].y + control[i][j + 1].y + control[i + 1][j + 1].y) / 3;
			index = min (K - 1, K * (h - minH) / (maxH - minH));
			glColor3f (palette[index].r / 255, palette[index].g / 255, palette[index].b / 255);
			glVertex3f (control[i + 1][j].x, control[i + 1][j].y, control[i + 1][j].z);
			glVertex3f (control[i][j + 1].x, control[i][j + 1].y, control[i][j + 1].z);
			glVertex3f (control[i + 1][j + 1].x, control[i + 1][j + 1].y, control[i + 1][j + 1].z);
		}
	}
	glEnd();
	glutSwapBuffers();
}


void keyboardEvent (unsigned char key, int x, int y)
{
	if (TM)
	{
		switch (key)
		{
			case ('2'):
				TM = false;
				glutSetWindowTitle ("Surface Editing Mode");
				break;
			case ('q'):
				--surfaceTranslate.x;
				break;
			case ('w'):
				++surfaceTranslate.x;
				break;
			case ('a'):
				--surfaceTranslate.y;
				break;
			case ('s'):
				++surfaceTranslate.y;
				break;
			case ('z'):
				--surfaceTranslate.z;
				break;
			case ('x'):
				++surfaceTranslate.z;
				break;
			case ('r'):
				++surfaceRotate;
				break;
			case ('t'):
				--surfaceRotate;
				break;
		}
	}
	else
	{
		switch (key)
		{
			case ('1'):
				TM = true;
				glutSetWindowTitle ("Transformation Mode");
				break;
			case ('q'):
				--points[currentPointX][currentPointY].x;
				updateControl();
				break;
			case ('w'):
				++points[currentPointX][currentPointY].x;
				updateControl();
				break;
			case ('a'):
				--points[currentPointX][currentPointY].y;
				updateControl();
				break;
			case ('s'):
				++points[currentPointX][currentPointY].y;
				updateControl();
				break;
			case ('z'):
				--points[currentPointX][currentPointY].z;
				updateControl();
				break;
			case ('x'):
				++points[currentPointX][currentPointY].z;
				updateControl();
				break;
			case ('i'):
				if (!(SampleR % 2))
				{
					SampleR /= 2;
					updateControl();
				}
				break;
			case ('o'):
				SampleR *= 2;
				updateControl();
				break;
			case ('k'):
				if (!(SampleC % 2))
				{
					SampleC /= 2;
					updateControl();
				}
				break;
			case ('l'):
				SampleC *= 2;
				updateControl();
				break;
		}
	}
	glutPostRedisplay();
}


void arrowKeys (int key, int x, int y)
{
	if (!TM)
	{
		switch (key)
		{
			case (GLUT_KEY_UP):
				if (currentPointY < N - 1)
				{
					++currentPointY;
				}
				break;
			case (GLUT_KEY_DOWN):
				if (currentPointY)
				{
					--currentPointY;
				}
				break;
			case (GLUT_KEY_LEFT):
				if (currentPointX)
				{
					--currentPointX;
				}
				break;
			case (GLUT_KEY_RIGHT):
				if (currentPointX < M - 1)
				{
					++currentPointX;
				}
				break;
		}
		glutPostRedisplay();
	}
}


void changeDirection (int x, int y)
{
	float dist = sqrt (pow (camera.x, 2) + pow (camera.y, 2) + pow (camera.z, 2));
	camera.x = dist * sin (360.0 / 800 * x * 0.0174532) * sin (360.0 / 800 * y * 0.0174532);
	camera.y = dist * cos (360.0 / 800 * y * 0.0174532);
	camera.z = dist * cos (360.0 / 800 * x * 0.0174532) * sin (360.0 / 800 * y * 0.0174532);
	up.x = dist * sin (360.0 / 800 * x * 0.0174532) * sin (360.0 / 800 * y * 0.0174532 - 1) - camera.x;
	up.y = dist * cos (360.0 / 800 * y * 0.0174532 - 1) - camera.y;
	up.z = dist * cos (360.0 / 800 * x * 0.0174532) * sin (360.0 / 800 * y * 0.0174532 - 1) - camera.z;
	glutPostRedisplay();
}


void mouseEvent (int key, int state, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
		changeDirection (x, y);
	}
}



void readFile (char *fname)
{
	ifstream file (fname);
	if (file.is_open())
	{
		file >> SampleR >> SampleC;
		file >> M >> N;
		for (int i = 0; i < M; ++i)
		{
			vector <vertex> tempv;
			for (int j = 0; j < N; ++j)
			{
				vertex temp;
				file >> temp.x >> temp.y >> temp.z;
				tempv.push_back (temp);
			}
			points.push_back (tempv);
		}
		file >> K;
		for (int i = 0; i < K; ++i)
		{
			RGB temp;
			file >> temp.r >> temp.g >> temp.b;
			palette.push_back (temp);
		}
	}
	file.close();
}


int main (int argc, char *argv[])
{
	surfaceTranslate.x = surfaceTranslate.y = surfaceTranslate.z = toY.x = toY.y = toY.z = up.x = up.z = 0;
	up.y = 1;
	camera.x = camera.y = camera.z = 100;
	readFile (argv[1]);
	updateControl();
	glutInit (&argc,argv);
	glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition (50, 50);
	glutInitWindowSize (800, 800);
	main_window = glutCreateWindow ("Transformation Mode");
	glutDisplayFunc (display);
	glutKeyboardFunc (keyboardEvent);
	glutSpecialFunc (arrowKeys);
	glutMouseFunc (mouseEvent);
	glutMotionFunc (changeDirection);
	initialDisplay();
	glutMainLoop();
}
