#include <GL/glut.h>
#include <iostream>
#include "Poly.h"
#include "Line.h"
#include "Helper.h"
#include <vector>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

static int menu_id;
static int submenu_id;
static int submenu_id2;
static int submenu_id3;
vector<Poly*> polygons;
int value;
int numPoly = 0;
int height = 300;
int width = 300;
int Win1, Win2, Win3; // window ids
float fx = 12; // from point
float fy = 12;
float fz = 12;
float kar = .8; // k ambient
float kag = .8;
float kab = .8;
float ksr = .8; // k specular
float ksg = .8;
float ksb = .8;
float kdr = .8; // k diffuse
float kdg = .8;
float kdb = .8;
float lx = 10; // light location
float ly = 10;
float lz = 10;
float Iar = .8; // intensity ambient
float Iag = .8;
float Iab = .8;
float ILr = .9; // intensity light
float ILg = .9;
float ILb = .9;
int PN = 3; // phong constant


//More pixel buffers!!
float *PixelBuffer;
float* Buffer1;
float* Buffer2;
float* Buffer3;

//and more display functions!!
void display();
void display1();
void display2();
void display3();

//void menu(unsigned char key, int x, int y);

void menu(int num);

void createMenu();
void update(int);

int main(int argc, char *argv[])
{
	//allocate new pixel buffer, need initialization!!
	PixelBuffer = new float[600 * 600 * 3];
	for (int i = 0; i < 600 * 600 * 3; i++)
	{
		PixelBuffer[i] = 0;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	//set window size to 600*600
	glutInitWindowSize(600, 600);
	//set window position
	glutInitWindowPosition(100, 100);
	PixelBuffer[0] = 0;
	PixelBuffer[1] = 0;
	PixelBuffer[2] = 0;
	//create and set main window title
	int MainWindow = glutCreateWindow("Hello Windows!!");

	createMenu();

	glClearColor(0, 0, 0, 0); //clears the buffer of OpenGL
							  //sets display function
	glutDisplayFunc(display);
	
	//allocate and initialize subwindow buffer
	Buffer1 = new float[300 * 300 * 3];
	Buffer2 = new float[300 * 300 * 3];
	Buffer3 = new float[300 * 300 * 3];
	for (int i = 0; i < 300 * 300 * 3; i++)
	{
		Buffer1[i] = 0;
		Buffer2[i] = 0;
		Buffer3[i] = 0;
	}


	//create subwindows
	//the 5 parameters are: main window ID, xpos, ypos, width, height
	Win1 = glutCreateSubWindow(MainWindow, 0, 300, 300, 300);
	glutDisplayFunc(display1);

	Win2 = glutCreateSubWindow(MainWindow, 0, 0, 300, 300);
	glutDisplayFunc(display2);

	Win3 = glutCreateSubWindow(MainWindow, 300, 0, 300, 300);
	glutDisplayFunc(display3);

	glutMainLoop();//main display loop, will display until terminate
	return 0;
}

//main display loop, this function will be called again and again by OpenGL
void display()
{
	//Misc.
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//draws pixel on screen, width and height must match pixel buffer dimension
	glDrawPixels(300, 300, GL_RGB, GL_FLOAT, PixelBuffer);

	//window refresh
	//glEnd();
	glFlush();
}

//additional display function for subwindows
void display1()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawPixels(300, 300, GL_RGB, GL_FLOAT, Buffer1);
	glFlush();
}

void display2()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawPixels(300, 300, GL_RGB, GL_FLOAT, Buffer2);
	glFlush();
}

void display3()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawPixels(300, 300, GL_RGB, GL_FLOAT, Buffer3);
	glFlush();
}


void removeAll()
{
	for (int i = 0; i < numPoly; i++)
	{
		delete polygons[0];
		polygons.erase(polygons.begin());
	}
	numPoly = 0;

	clearBuffer(Buffer1, height, width);
	clearBuffer(Buffer2, height, width);
	clearBuffer(Buffer3, height, width);
	glutSetWindow(Win1);
	glutPostRedisplay();
	glutSetWindow(Win2);
	glutPostRedisplay();
	glutSetWindow(Win3);
	glutPostRedisplay();
}

void sortXY()
{
	Poly* temp;
	for (int i = 0; i < numPoly; i++)
	{
		for(int j = 0; j < numPoly-1; j++)
			if (polygons[j]->getsz() < polygons[j+1]->getsz())
			{
				temp = polygons[j];
				polygons[j] = polygons[j+1];
				polygons[j+1] = temp;
			}
	}
}

void sortYZ()
{
	Poly* temp;
	for (int i = 0; i < numPoly; i++)
	{
		for(int j = 0; j < numPoly-1; j++)
			if (polygons[j]->getsx() < polygons[j+1]->getsx())
			{
				temp = polygons[j];
				polygons[j] = polygons[j+1];
				polygons[j+1] = temp;
			}
	}
}

void sortXZ()
{
	Poly* temp;
	for (int i = 0; i < numPoly; i++)
	{
		for(int j = 0; j < numPoly-1; j++)
			if (polygons[j]->getsy() < polygons[j+1]->getsy())
			{
				temp = polygons[j];
				polygons[j] = polygons[j+1];
				polygons[j+1] = temp;
			}
	}
}

void normalizeColor()
{
	float gminr = 0, gmaxr = 0;
	float gming = 0, gmaxg = 0;
	float gminb = 0, gmaxb = 0; //global min/maxes
	float tminr, tmaxr, tming, tmaxg, tminb, tmaxb; //temp min/max for each poly
	
	polygons[0]->getMinMaxC(gminr, gmaxr, gming, gmaxg, gminb, gmaxb);
	
	for (int i = 1; i < numPoly; i++)
	{
		polygons[i]->getMinMaxC(tminr, tmaxr, tming, tmaxg, tminb, tmaxb);
		if (tminr < gminr)
			gminr = tminr;
		if (tmaxr > gmaxr)
			gmaxr = tmaxr;

		if (tming < gming)
			gming = tming;
		if (tmaxg > gmaxg)
			gmaxg = tmaxg;

		if (tminb < gminb)
			gminb = tminb;
		if (tmaxb > gmaxb)
			gmaxb = tmaxb;

	}


	float dr = gmaxr - gminr;
	float dg = gmaxg - gming;
	float db = gmaxb - gminb;
		
	for (int i = 0; i < numPoly; i++) // normalize colors for each polyhedron
		polygons[i]->ndcC(dr, dg, db, gminr, gming, gminb);
}

void phong()
{
	for (int i = 0; i < numPoly; i++)
	{
		polygons[i]->phong(fx, fy, fz, kar, kag, kab, ksr, ksg, ksb, kdr, kdg, kdb, lx, ly, lz, Iar, Iag, Iab, ILr, ILg, ILb, PN);
	}
	normalizeColor();
}

void normalizeAll()
{
	float gminx = 0, gmaxx = 0;
	float gminy = 0, gmaxy = 0;
	float gminz = 0, gmaxz = 0; //global min/maxes
	float tminx, tmaxx, tminy, tmaxy, tminz, tmaxz; //temp min/max for each poly
	
	polygons[0]->getMinMax(gminx, gmaxx, gminy, gmaxy, gminz, gmaxz);
	
	for (int i = 1; i < numPoly; i++)
	{
		polygons[i]->getMinMax(tminx, tmaxx, tminy, tmaxy, tminz, tmaxz);
		if (tminx < gminx)
			gminx = tminx;
		if (tmaxx > gmaxx)
			gmaxx = tmaxx;

		if (tminy < gminy)
			gminy = tminy;
		if (tmaxy > gmaxy)
			gmaxy = tmaxy;

		if (tminz < gminz)
			gminz = tminz;
		if (tmaxz > gmaxz)
			gmaxz = tmaxz;

	}

	float dx = gmaxx - gminx;
	float dy = gmaxy - gminy;
	float dz = gmaxz - gminz;
	float mdelta = dx;
	
	if (dy > mdelta)
		mdelta = dy;
	if (dz > mdelta)
		mdelta = dz;

	for (int i = 0; i < numPoly; i++)
		polygons[i]->ndcTriangs(mdelta, gminx, gminy, gminz, 300);
}

void projectAll()
{
	if (numPoly == 0)
	{
		return;
	}

	clearBuffer(Buffer1, height, width);
	clearBuffer(Buffer2, height, width);
	clearBuffer(Buffer3, height, width);

	sortXY();
	for (int i = numPoly-1; i >= 0; i--)
	{
		polygons[i]->rasterizeXY(Buffer1, height, width);
	}

	sortYZ();
	for (int i = numPoly - 1; i >= 0; i--)
	{
		polygons[i]->rasterizeYZ(Buffer2, height, width);
	}

	sortXZ();
	for (int i = numPoly - 1; i >= 0; i--)
	{
		polygons[i]->rasterizeXZ(Buffer3, height, width);
	}

	glutSetWindow(Win1);
	glutPostRedisplay();
	glutSetWindow(Win2);
	glutPostRedisplay();
	glutSetWindow(Win3);
	glutPostRedisplay();
}

void LoadFile()
{
	ifstream infile;
	string fname = "";
	int currnumv, currnumadj, tind, tadj, numtriang, i1, i2, i3;
	float nx, ny, nz;
	cout << "Enter input filename: ";
	cin >> fname;
	
	cout << "Opening " << fname << endl;
	infile.open(fname.c_str());

	if (!infile)
	{
		cout << "Invalid Filename" << endl;
		return;
	}

	removeAll();
	infile >> numPoly;

	for (int i = 0; i < numPoly; i++)
	{
		infile >> currnumv; // read in vertices
		float* xs = new float[currnumv];
		float* ys = new float[currnumv];
		float* zs = new float[currnumv];
		for (int j = 0; j < currnumv; j++)
		{
			infile >> xs[j] >> ys[j] >> zs[j];
		}

		Poly* addpoly = new Poly(currnumv, xs, ys, zs);

		infile >> currnumadj; // read in adjacencies
		addpoly->setNumAdj(currnumadj);
		for (int j = 0; j < currnumadj; j++)
		{
			infile >> tind >> tadj;
			addpoly->setAdj(tind-1, tadj-1);
		}

		infile >> numtriang;
		addpoly->setNumTriang(numtriang);

		for (int j = 0; j < numtriang; j++) // read triangles based on vertex indices
		{
			infile >> i1 >> i2 >> i3;
			addpoly->makeTriang(i1-1, i2-1, i3-1);
		}

		for (int j = 0; j < numtriang; j++) // set normals of surface
		{
			infile >> nx >> ny >> nz;
			addpoly->setNormal(j, nx, ny, nz);
		}

		polygons.insert(polygons.end(), addpoly);

		delete xs;
		delete ys;
		delete zs;
	}

	infile.close();
	normalizeAll();
	phong();
	projectAll();
}

void changeIntensity()
{
	cout << "Please enter I_a as r g b: ";
	cin >> Iar >> Iag >> Iab;
	cout << "Please enter I_L as r g b: ";
	cin >> ILr >> ILg >> ILb;

	phong();
	projectAll();
}

void changeFPoint()
{
	cout << "Please enter fx fy fz: ";
	cin >> fx >> fy >> fz;
	
	phong();
	projectAll();
}

void changeKval()
{
	cout << "Please enter k ambient as r g b: ";
	cin >> kar >> kag >> kab;
	
	cout << "Please enter k specular as r g b: ";
	cin >> ksr >> ksg >> ksb;
	
	cout << "Please enter k diffuse as r g b: ";
	cin >> kdr >> kdg >> kdb;

	phong();
	projectAll();
}

void changeLight()
{
	cout << "Please enter light coordinate as x y z: ";
	cin >> lx >> ly >> lz;
	
	phong();
	projectAll();
}

void changePhongConstant()
{
	cout << "Please enter integer for Phong constant: ";
	cin >> PN;
	
	phong();
	projectAll();
}

void HalftoneOn()
{
	Halftone(Buffer1, height, width);
	Halftone(Buffer2, height, width);
	Halftone(Buffer3, height, width);
	glutSetWindow(Win1);
	glutPostRedisplay();
	glutSetWindow(Win2);
	glutPostRedisplay();
	glutSetWindow(Win3);
	glutPostRedisplay();
}

void menu(int num) {
	switch (num)
	{
	case 1: cout << "Removing all objects." << endl;
		removeAll();
		break;
	case 2: changeIntensity();
		break;
	case 3: LoadFile();
		break;
	case 4: changeFPoint();
		break;
	case 5: changeKval();
		break;
	case 6: changeLight();
		break;
	case 7: changePhongConstant();
		break;
	case 8: HalftoneOn();
		break;
	case 9: projectAll();
		break;
	default: cout << "Invalid Option" << endl;
	}

}

void createMenu() {
	submenu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Change Intensity", 2);
	glutAddMenuEntry("Change From Point", 4);
	glutAddMenuEntry("Change K values", 5);
	glutAddMenuEntry("Change Light Location", 6);
	glutAddMenuEntry("Change Phong Constant", 7);

	submenu_id2 = glutCreateMenu(menu);
	glutAddMenuEntry("Clear All", 1);
	glutAddMenuEntry("Load File", 3);

	submenu_id3 = glutCreateMenu(menu);
	glutAddMenuEntry("Half Tone On", 8);
	glutAddMenuEntry("Half Tone Off", 9);

	menu_id = glutCreateMenu(menu);
	glutAddSubMenu("IO", submenu_id2);
	glutAddSubMenu("Phong Parameters", submenu_id);
	glutAddSubMenu("Halftone", submenu_id3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

