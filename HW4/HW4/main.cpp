#include <GL/glut.h>
#include "Helper.h"
#include "Line.h"
#include "Bezier.h"
#include "Spline.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

float *PixelBuffer;
void display();
int numbez = 0;
int numspline = 0;
int height = 400;
int width = 400;
static int submenu_id;
static int submenu_id2;
static int submenu_id3;
static int menu_id;
int resolution = 50;
vector<Bezier*> bezcurves;
vector<Spline*> splinecurves;

void createMenu();
void normalizeAll();
void ProjectAll();

void main(int argc, char *argv[])
{
	//allocate new pixel buffer, need initialization!!
	PixelBuffer = new float[400 * 400 * 3];
	clearBuffer(PixelBuffer, 400, 400);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	//set window size to 200*200
	glutInitWindowSize(400, 400);
	//set window position
	glutInitWindowPosition(100, 100);

	//create and set main window title
	int MainWindow = glutCreateWindow("Hello Graphics!!");

	createMenu();
	glClearColor(0, 0, 0, 0); //clears the buffer of OpenGL
							  //sets display function
	glutDisplayFunc(display);

	glutMainLoop();//main display loop, will display until terminate
	return;
}

//main display loop, this function will be called again and again by OpenGL
void display()
{
	//Misc.
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//draws pixel on screen, width and height must match pixel buffer dimension
	glDrawPixels(width, height, GL_RGB, GL_FLOAT, PixelBuffer);

	//window refresh
	glEnd();
	glFlush();
}

void removeAll()
{
	for (int i = 0; i < numbez; i++)
	{
		delete bezcurves[0];
		bezcurves.erase(bezcurves.begin());
	}
	numbez = 0;

	for (int i = 0; i < numspline; i++)
	{
		delete splinecurves[0];
		splinecurves.erase(splinecurves.begin());
	}
	numspline = 0;

	clearBuffer(PixelBuffer, height, width);
	glutPostRedisplay();
}

void normalizeAll()
{
	if (numbez == 0 && numspline == 0)
		return;

	float gminx = 0, gmaxx = 0;
	float gminy = 0, gmaxy = 0; //global min/maxes
	float tminx, tmaxx, tminy, tmaxy, tminz, tmaxz; //temp min/max for each poly

	if(numbez > 0)
		bezcurves[0]->getMinMax(gminx, gmaxx, gminy, gmaxy);
	else
		splinecurves[0]->getMinMax(gminx, gmaxx, gminy, gmaxy);

	//cout << "Bez vals" << endl;

	for (int i = 0; i < numbez; i++)
	{
		bezcurves[i]->getMinMax(tminx, tmaxx, tminy, tmaxy);

		//cout << tminx << " " << tmaxx << " " << tminy << " " << tmaxy << endl;

		if (tminx < gminx)
			gminx = tminx;
		if (tmaxx > gmaxx)
			gmaxx = tmaxx;

		if (tminy < gminy)
			gminy = tminy;
		if (tmaxy > gmaxy)
			gmaxy = tmaxy;

	}

	//cout << "Spline vals" << endl;
	for (int i = 0; i < numspline; i++)
	{
		splinecurves[i]->getMinMax(tminx, tmaxx, tminy, tmaxy);

		//cout << tminx << " " << tmaxx << " " << tminy << " " << tmaxy << endl;

		if (tminx < gminx)
			gminx = tminx;
		if (tmaxx > gmaxx)
			gmaxx = tmaxx;

		if (tminy < gminy)
			gminy = tminy;
		if (tmaxy > gmaxy)
			gmaxy = tmaxy;

	}

	float dx = gmaxx - gminx;
	float dy = gmaxy - gminy;
	float mdelta = dx;

	if (dy > mdelta)
		mdelta = dy;

	//cout << "gmins" << endl;
	//cout << gminx << " " << gminy << endl;
	//cout << mdelta << endl;

	for (int i = 0; i < numbez; i++)
	{
		bezcurves[i]->ndcAll(gminx, gminy, mdelta, width);
	}

	for (int i = 0; i < numspline; i++)
	{
		splinecurves[i]->ndcAll(gminx, gminy, mdelta, width);
	}

}

void LoadFile()
{
	ifstream infile;
	string fname = "";
	string type = "";
	int numcurves, nump, kval;
	char tk; // whether knots specified
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

	infile >> numcurves;

	for (int i = 0; i < numcurves; i++)
	{
		infile >> type;
		
		if (type == "bz") // bezier curve
		{
			infile >> nump;
			float* txs = new float[nump];
			float* tys = new float[nump];
			for (int j = 0; j < nump; j++)
			{
				infile >> txs[j] >> tys[j];
			}
			Bezier* addbez = new Bezier(nump, txs, tys);
			bezcurves.push_back(addbez);
			numbez++;

			delete txs;
			delete tys;
		}

		else // is a spline curve
		{
			infile >> nump;
			infile >> kval;
			infile >> tk;
			float* txs = new float[nump];
			float* tys = new float[nump];
			for (int j = 0; j < nump; j++)
			{
				infile >> txs[j] >> tys[j];
				//cout << txs[i] << " " << tys[i] << endl;
			}

			if (tk == 'T')
			{
				float* tknots = new float[nump + kval];
				for (int j = 0; j < nump + kval; j++)
				{
					infile >> tknots[j];
				}
				Spline* addspline = new Spline(nump, kval, txs, tys, tknots);
				splinecurves.push_back(addspline);
				delete tknots;
			}

			else // knots not specified
			{
				Spline* addspline = new Spline(nump, kval, txs, tys);
				splinecurves.push_back(addspline);
			}
			numspline++;

			delete txs;
			delete tys;
		}
	}

	infile.close();
	normalizeAll();
	ProjectAll();
}

void ProjectAll()
{
	clearBuffer(PixelBuffer, height, width);

	for (int i = 0; i < numbez; i++)
	{
		bezcurves[i]->draw(PixelBuffer, width, resolution);
	}

	for (int i = 0; i < numspline; i++)
	{
		splinecurves[i]->draw(PixelBuffer, width, resolution);
	}

	glutPostRedisplay();
}

void AddBezierPoint()
{
	int index, pindex;
	float nx, ny;
	cout << "Please enter index between 0 and " << numbez - 1 << " inclusive: ";
	cin >> index;

	if (index < 0 || index > numbez - 1)
	{
		cout << "Invalid index" << endl;
		return;
	}
	
	bezcurves[index]->printcp();
	cout << endl << "Enter the index BEFORE which to insert the new control point: ";
	cin >> pindex;

	cout << "Enter new control point as x y: ";
	cin >> nx >> ny;

	bezcurves[index]->addcp(pindex, nx, ny);
	normalizeAll();
	ProjectAll();
}

void ModBezierPoint()
{
	int index, pindex;
	float nx, ny;
	cout << "Please enter index between 0 and " << numbez - 1 << " inclusive: ";
	cin >> index;

	if (index < 0 || index > numbez - 1)
	{
		cout << "Invalid index" << endl;
		return;
	}

	bezcurves[index]->printcp();
	cout << endl << "Enter the index of control point to modify: ";
	cin >> pindex;

	cout << "Enter new control point as x y: ";
	cin >> nx >> ny;

	bezcurves[index]->changecp(pindex, nx, ny);
	normalizeAll();
	ProjectAll();
}

void DelBezierPoint()
{
	int index, pindex;
	float nx, ny;
	cout << "Please enter index between 0 and " << numbez - 1 << " inclusive: ";
	cin >> index;

	if (index < 0 || index > numbez - 1)
	{
		cout << "Invalid index" << endl;
		return;
	}

	bezcurves[index]->printcp();
	cout << endl << "Enter the index of control point to delete: ";
	cin >> pindex;

	bezcurves[index]->deletecp(pindex);
	normalizeAll();
	ProjectAll();
}

void ModifyKnots()
{
	int index, ncp, k;
	cout << "Please enter index between 0 and " << numspline - 1 << " inclusive: ";
	cin >> index;

	if (index < 0 || index > numspline - 1)
	{
		cout << "Invalid index" << endl;
		return;
	}

	splinecurves[index]->printcpknots();
	k = splinecurves[index]->getk();
	ncp = splinecurves[index]->getnumcp();
	cout << "Please enter new " << ncp + k << " knots" << endl;

	float* tknots = new float[ncp + k];
	for (int i = 0; i < ncp + k; i++)
	{
		cout << "Enter knot " << i << ": ";
		cin >> tknots[i];
	}

	splinecurves[index]->modifyknots(tknots);
	delete tknots;
	ProjectAll();
}


void ModifyKval()
{
	int index, ncp, k;
	cout << "Please enter index between 0 and " << numspline - 1 << " inclusive: ";
	cin >> index;

	if (index < 0 || index > numspline - 1)
	{
		cout << "Invalid index" << endl;
		return;
	}

	splinecurves[index]->printcpknots();

	cout << "Enter new k: ";
	cin >> k;

	splinecurves[index]->setk(k);
	ncp = splinecurves[index]->getnumcp();
	cout << "Please enter new " << ncp + k << " knots" << endl;

	float* tknots = new float[ncp + k];
	for (int i = 0; i < ncp + k; i++)
	{
		cout << "Enter knot " << i << ": ";
		cin >> tknots[i];
	}

	splinecurves[index]->modifyknots(tknots);

	delete tknots;
	ProjectAll();
}

void ChangeResolution()
{
	int nres;
	cout << "Enter a new resolution: ";
	cin >> nres;
	if (nres <= 1)
	{
		cout << "Invalid resolution";
		return;
	}
	resolution = nres;
	ProjectAll();
}

void SaveFile()
{
	ofstream outfile;
	string fname = "";
	cout << "Enter output filename: ";
	cin >> fname;

	cout << "Saving to " << fname << endl;
	outfile.open(fname.c_str());

	for (int i = 0; i < numbez; i++)
	{
		bezcurves[i]->tofile(outfile);
	}

	for (int i = 0; i < numspline; i++)
	{
		splinecurves[i]->tofile(outfile);
	}
}

void ChangeSplinePoint()
{
	int index, pindex;
	float nx, ny;
	cout << "Please enter index between 0 and " << numspline - 1 << " inclusive: ";
	cin >> index;

	if (index < 0 || index > numspline - 1)
	{
		cout << "Invalid index" << endl;
		return;
	}

	splinecurves[index]->printcpknots();

	cout << "Enter index of control point to modify: ";
	cin >> pindex;

	cout << "Enter new control point as x y: ";
	cin >> nx >> ny;

	splinecurves[index]->modifycp(pindex, nx, ny);

	normalizeAll();
	ProjectAll();
}

void AddSplinePoint()
{
	int index, ncp, k;
	float nx, ny;
	cout << "Please enter index between 0 and " << numspline - 1 << " inclusive: ";
	cin >> index;

	if (index < 0 || index > numspline - 1)
	{
		cout << "Invalid index" << endl;
		return;
	}

	splinecurves[index]->printcpknots();

	cout << "Enter new control point as x y: ";
	cin >> nx >> ny;

	splinecurves[index]->addcp(nx, ny);
	k = splinecurves[index]->getk();
	ncp = splinecurves[index]->getnumcp();

	cout << "Please enter new " << ncp + k << " knots" << endl;

	float* tknots = new float[ncp + k];
	for (int i = 0; i < ncp + k; i++)
	{
		cout << "Enter knot " << i << ": ";
		cin >> tknots[i];
	}

	splinecurves[index]->modifyknots(tknots);
	delete tknots;

	normalizeAll();
	ProjectAll();
}

void DeleteSplinePoint()
{
	int index, pindex, ncp, k;
	cout << "Please enter index between 0 and " << numspline - 1 << " inclusive: ";
	cin >> index;

	if (index < 0 || index > numspline - 1)
	{
		cout << "Invalid index" << endl;
		return;
	}

	splinecurves[index]->printcpknots();

	cout << "Enter index of control point to delete: ";
	cin >> pindex;

	splinecurves[index]->deletecp(pindex);
	k = splinecurves[index]->getk();
	ncp = splinecurves[index]->getnumcp();

	cout << "Please enter new " << ncp + k << " knots" << endl;

	float* tknots = new float[ncp + k];
	for (int i = 0; i < ncp + k; i++)
	{
		cout << "Enter knot " << i << ": ";
		cin >> tknots[i];
	}

	splinecurves[index]->modifyknots(tknots);
	delete tknots;

	normalizeAll();
	ProjectAll();
}


void menu(int num) {
	switch (num)
	{
	case 1: cout << "Removing all objects." << endl;
		removeAll();
		break;
	case 2: AddBezierPoint();
		break;
	case 3: LoadFile();
		break;
	case 4: ModBezierPoint();
		break;
	case 5: DelBezierPoint();
		break;
	case 6: ModifyKnots();
		break;
	case 7: ModifyKval();
		break;
	case 8: ChangeResolution();
		break;
	case 9: SaveFile();
		break;
	default: cout << "Invalid Option" << endl;
	}

}

void createMenu() {
	submenu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Add Control Point", 2);
	glutAddMenuEntry("Modify Control Point", 4);
	glutAddMenuEntry("Delete Control Point", 5);

	submenu_id2 = glutCreateMenu(menu);
	glutAddMenuEntry("Clear All", 1);
	glutAddMenuEntry("Load File", 3);
	glutAddMenuEntry("Save File", 9);
	glutAddMenuEntry("Change Resolution", 8);

	submenu_id3 = glutCreateMenu(menu);
	glutAddMenuEntry("Modify knots", 6);
	glutAddMenuEntry("Modify K", 7);

	menu_id = glutCreateMenu(menu);
	glutAddSubMenu("General", submenu_id2);
	glutAddSubMenu("Bezier", submenu_id);
	glutAddSubMenu("B-Spline", submenu_id3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}