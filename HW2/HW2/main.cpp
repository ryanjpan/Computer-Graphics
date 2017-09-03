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
vector<Poly*> polygons;
int value;
int numPoly = 0;
int height = 200;
int width = 200;
int Win1, Win2, Win3; // window ids
bool anim = true;

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
	PixelBuffer = new float[400 * 400 * 3];
	for (int i = 0; i < 400 * 400 * 3; i++)
	{
		PixelBuffer[i] = 0;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	//set window size to 200*200
	glutInitWindowSize(400, 400);
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
	Buffer1 = new float[200 * 200 * 3];
	Buffer2 = new float[200 * 200 * 3];
	Buffer3 = new float[200 * 200 * 3];
	for (int i = 0; i < 200 * 200 * 3; i++)
	{
		Buffer1[i] = 0;
		Buffer2[i] = 0;
		Buffer3[i] = 0;
	}


	//create subwindows
	//the 5 parameters are: main window ID, xpos, ypos, width, height
	Win1 = glutCreateSubWindow(MainWindow, 0, 200, 200, 200);
	glutDisplayFunc(display1);

	Win2 = glutCreateSubWindow(MainWindow, 0, 0, 200, 200);
	glutDisplayFunc(display2);

	Win3 = glutCreateSubWindow(MainWindow, 200, 0, 200, 200);
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
	glDrawPixels(200, 200, GL_RGB, GL_FLOAT, PixelBuffer);

	//window refresh
	//glEnd();
	glFlush();
}

//additional display function for subwindows
void display1()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawPixels(200, 200, GL_RGB, GL_FLOAT, Buffer1);
	glFlush();
}

void display2()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawPixels(200, 200, GL_RGB, GL_FLOAT, Buffer2);
	glFlush();
}

void display3()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawPixels(200, 200, GL_RGB, GL_FLOAT, Buffer3);
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

void projectAll()
{
	if (numPoly == 0)
	{
		return;
	}

	clearBuffer(Buffer1, height, width);
	clearBuffer(Buffer2, height, width);
	clearBuffer(Buffer3, height, width);

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
	{
		polygons[i]->projectXY(Buffer1, height, width, mdelta, gminx, gminy);
		polygons[i]->projectYZ(Buffer2, height, width, mdelta, gminy, gminz);
		polygons[i]->projectXZ(Buffer3, height, width, mdelta, gminx, gminz);
	}

	glutSetWindow(Win1);
	glutPostRedisplay();
	glutSetWindow(Win2);
	glutPostRedisplay();
	glutSetWindow(Win3);
	glutPostRedisplay();
}

void ProjectAxis(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float mdelta, float gminx, float gminy, float gminz)
{
	int n1x, n1y, n1z, n2x, n2y, n2z;
	
	changeColor(1, 0, 0);
	
	n1x = round((p1x - gminx) / mdelta * (width - 1));
	n1y = round((p1y - gminy) / mdelta * (width - 1));
	n1z = round((p1z - gminz) / mdelta * (width - 1));
	
	n2x = round((p2x - gminx) / mdelta * (width - 1));
	n2y = round((p2y - gminy) / mdelta * (width - 1));
	n2z = round((p2z - gminz) / mdelta * (width - 1));
	
	Line temp(n1x, n1y, n2x, n2y);
	temp.ddaDraw(Buffer1, width);
	Line temp2(n1y, n1z, n2y, n2z);
	temp2.ddaDraw(Buffer2, width);
	Line temp3(n1x, n1z, n2x, n2z);
	temp3.ddaDraw(Buffer3, width);
	
	changeColor(.4, 0, 8);
	
}

void projectAllRotation(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z) //Project that includes an axis
{
	if (numPoly == 0)
	{
		return;
	}

	clearBuffer(Buffer1, height, width);
	clearBuffer(Buffer2, height, width);
	clearBuffer(Buffer3, height, width);

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
	
	if(p1x < gminx) //following if statements for checking the axis points
		gminx = p1x;
	if(p2x < gminx)
		gminx = p2x;
	if(p1x > gmaxx)
		gmaxx = p1x;
	if(p2x > gmaxx)
		gmaxx = p2x;
		
	if(p1y < gminy)
		gminy = p1y;
	if(p2y < gminy)
		gminy = p2y;
	if(p1y > gmaxy)
		gmaxy = p1y;
	if(p2y > gmaxy)
		gmaxy = p2y;
		
	if(p1z < gminz)
		gminz = p1z;
	if(p2z < gminz)
		gminz = p2z;
	if(p1z > gmaxz)
		gmaxz = p1z;
	if(p2z > gmaxz)
		gmaxz = p2z;

	float dx = gmaxx - gminx;
	float dy = gmaxy - gminy;
	float dz = gmaxz - gminz;
	float mdelta = dx;
	
	if (dy > mdelta)
		mdelta = dy;
	if (dz > mdelta)
		mdelta = dz;


	for (int i = 0; i < numPoly; i++)
	{
		polygons[i]->projectXY(Buffer1, height, width, mdelta, gminx, gminy);
		polygons[i]->projectYZ(Buffer2, height, width, mdelta, gminy, gminz);
		polygons[i]->projectXZ(Buffer3, height, width, mdelta, gminx, gminz);
	}
	
	ProjectAxis(p1x, p1y, p1z, p2x, p2y, p2z, mdelta, gminx, gminy, gminz);

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
	int currnumv, currnumadj, tind, tadj;
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
		infile >> currnumv;
		float* xs = new float[currnumv];
		float* ys = new float[currnumv];
		float* zs = new float[currnumv];
		for (int j = 0; j < currnumv; j++)
		{
			infile >> xs[j] >> ys[j] >> zs[j];
		}

		Poly* addpoly = new Poly(currnumv, xs, ys, zs);

		infile >> currnumadj;
		addpoly->setNumAdj(currnumadj);
		for (int j = 0; j < currnumadj; j++)
		{
			infile >> tind >> tadj;
			addpoly->setAdj(tind-1, tadj-1);
		}


		polygons.insert(polygons.end(), addpoly);

		delete xs;
		delete ys;
		delete zs;
	}

	infile.close();
	projectAll();
}

void SaveFile()
{
	ofstream outfile;
	string fname = "";
	int currnumv, currnumadj;
	cout << "Enter output filename: ";
	cin >> fname;

	cout << "Saving to file:" << fname << endl;
	outfile.open(fname.c_str());

	outfile << numPoly << endl << endl;

	for (int i = 0; i < numPoly; i++)
	{
		currnumv = polygons[i]->getNumVertex();
		outfile << currnumv << endl;
		for (int j = 0; j < currnumv; j++)
		{
			outfile << polygons[i]->getx(j) << " " << polygons[i]->gety(j);
			outfile << " " << polygons[i]->getz(j) << endl;
		}

		currnumadj = polygons[i]->getNumAdj();
		outfile << currnumadj << endl;
		polygons[i]->printAdj(outfile); // print the adjacencies
		outfile << endl;
	}

	outfile.close();
}

void Translation()
{
	float tx, ty, tz;
	int ind;

	cout << "Enter index of polyhedron to be translated: ";
	cin >> ind;
	cout << "Enter translation vector tx ty tz: ";
	cin >> tx >> ty >> tz;

	polygons[ind]->translate(tx, ty, tz);
	projectAll();
}

void Scale()
{
	int ind;
	float alpha, beta, gamma;

	cout << "Enter index of polyhedron to be scaled: ";
	cin >> ind;
	cout << "Enter scaling factors alpha beta gamma: ";
	cin >> alpha >> beta >> gamma;

	polygons[ind]->scale(alpha, beta, gamma);
	projectAll();
}

void Rotate()
{
	int ind;
	double deg;
	float p1x, p1y, p1z, p2x, p2y, p2z;
	
	cout << "Enter index of polyhedron to be rotated: ";
	cin >> ind;
	cout << "Enter first point of axis: ";
	cin >> p1x >> p1y >> p1z;
	cout << "Enter second point of axis: ";
	cin >> p2x >> p2y >> p2z;
	cout << "Enter degrees of rotation: ";
	cin >> deg;
	
	polygons[ind]->rotate(deg, p1x, p1y, p1z, p2x, p2y, p2z);
	projectAllRotation(p1x, p1y, p1z, p2x, p2y, p2z);
}

void AddPoly()
{
	int cnumv, cnumadj, v1, v2;
	cout << "Enter number of vertices: ";
	cin >> cnumv;
	
	float* txs = new float[cnumv];
	float* tys = new float[cnumv];
	float* tzs = new float[cnumv];
	
	cout << "Enter vertices as x y z" << endl;
	
	for(int i = 0; i < cnumv; i++)
	{
		cout << "Enter vertex " << i+1 << ": ";
		cin >> txs[i] >> tys[i] >> tzs[i];
	}
	
	Poly* addpoly = new Poly(cnumv, txs, tys, tzs);
	cout << endl << "Enter number of adjacencies: ";
	cin >> cnumadj;
	
	cout << "Enter adjacencies as index_vertex_1 index_vertex_2" << endl;
	
	for(int i = 0; i < cnumadj; i++)
	{
		cout << "Enter adjacency " << i+1 << ": ";
		cin >> v1 >> v2;
		addpoly->setAdj(v1-1, v2-1);
	}
	
	polygons.insert(polygons.end(), addpoly);
	numPoly++;
	projectAll();
	
}

void update(int)
{
	if(!anim)
		return;
	
	for(int i = 0; i < numPoly; i++)
		polygons[i]->rotate(5, 0, 0, 0, 1, 1, 1);
		
	projectAll();
	glutTimerFunc(100, update, 0);
}

void AnimOn()
{
	glutTimerFunc(100, update, 0); //register callback
	anim = true;
}

void AnimOff()
{
	anim = false;
}

void ShowCoord()
{
	int ind;
	cout << "Enter index of polyhedron: ";
	cin >> ind;
	cout << "Showing coordinates for polyhedron " << ind << endl;
	polygons[ind]->showCoord();
}

void menu(int num) {
	switch (num)
	{
	case 1: cout << "Removing all objects." << endl;
		removeAll();
		break;
	case 2: Translation();
		break;
	case 3: LoadFile();
		break;
	case 4: SaveFile();
		break;
	case 5: Scale();
		break;
	case 6: Rotate();
		break;
	case 7: AddPoly();
		break;
	case 8: AnimOn();
		break;
	case 9: AnimOff();
		break;
	case 10: ShowCoord();
		break;
	default: cout << "Invalid Option" << endl;
	}

}

void createMenu() {
	submenu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Translate", 2);
	glutAddMenuEntry("Scale", 5);
	glutAddMenuEntry("Rotate", 6);

	submenu_id2 = glutCreateMenu(menu);
	glutAddMenuEntry("Load File", 3);
	glutAddMenuEntry("Save File", 4);
	glutAddMenuEntry("Add Polyhedron", 7);
	glutAddMenuEntry("Show Coordinates", 10);

	menu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Clear All", 1);
	glutAddMenuEntry("Start Animation", 8);
	glutAddMenuEntry("Stop Animation", 9);
	glutAddSubMenu("Transformations", submenu_id);
	glutAddSubMenu("IO", submenu_id2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

