#include <GL/glut.h>
#include "Helper.h"
#include "Line.h"
#include "Poly.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

float *PixelBuffer;
void display();
int height, width;
int menu_id;
vector<Poly*> polygons;
vector<Line*> lines;
int value;
int numPoly = 0;
int numLines = 0;

void menu(unsigned char key, int x, int y);


int main(int argc, char *argv[])
{
	cout << "Enter the height then width" << endl;
	cin >> height >> width;
	
	//allocate new pixel buffer, need initialization!!
	PixelBuffer = new float[height * width * 3];

	clearBuffer(PixelBuffer, height, width);

	//Line test(1, 1, 100, 100);
	//test.ddaDraw(PixelBuffer, 200);
	//test.clip(50, 110, 50, 110);
	//test.ddaDraw(PixelBuffer, 200);

	//int xs[] = { 100, 150, 125, 75, 50 };
	//int ys[] = { 150, 100, 50, 50, 100 };
	
	//Poly test(5, xs, ys);
	//test.translate(-25, 0);
	//test.rasterize(PixelBuffer, 200, 200);

	//int xs2[] = { 50, 100, 100, 50 };
	//int ys2[] = { 50, 50, 100, 100 };
	
	//int xs3[] = { 50, 75, 90, 120, 160 };
	//int ys3[] = { 50, 150, 90, 130, 50 };
	
	//int xs4[] = {50, 50, 75, 100, 125, 150, 150};
	//int ys4[] = {50, 100, 100, 125, 100, 100, 50};

	//Poly* test2 = new Poly(4, xs2, ys2);
	//polygons.insert(polygons.end(), test2);
	//test2->draw(PixelBuffer, width);
	//test2->clip(40, 80, 10, 60);
	//test2->rasterize(PixelBuffer, width, height);
	//numPoly++;
	//test2.scale(2, 2);
	//test2.rotate(45);
	//test2.draw(PixelBuffer, 200);
	//test2.rasterize(PixelBuffer, 200, 200);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	//set window size to height, width
	glutInitWindowSize(width, height);
	//set window position
	glutInitWindowPosition(100, 100);

	//create and set main window title
	int MainWindow = glutCreateWindow("Hello Graphics!!");

	glutKeyboardFunc(menu);
	glClearColor(0, 0, 0, 0); //clears the buffer of OpenGL
						

	glutDisplayFunc(display);//sets display function

	glutMainLoop();//main display loop, will display until terminate
	cout << "back in main" << endl;
	return 0;
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
	for (int i = 0; i < numPoly; i++)
	{
		delete polygons[0];
		polygons.erase(polygons.begin());
	}

	for (int i = 0; i < numLines; i++)
	{
		delete lines[0];
		lines.erase(lines.begin());
	}

	numPoly = 0;
	numLines = 0;
}

void redrawAll()
{
	clearBuffer(PixelBuffer, height, width);
	for(int i = 0; i < numPoly; i++)
	{
		polygons[i]->rasterize(PixelBuffer, width, height);
	}
	
	for(int i = 0; i < numLines; i++)
	{
		lines[i]->ddaDraw(PixelBuffer, width);
	}
}

void LineDraw(int choice)
{
	int x1 = 0;
	int x2, y1, y2;
	cout << "Enter first endpoint: ";
	cin >> x1 >> y1;
	cout << "Enter second endpoint: ";
	cin >> x2 >> y2;
	
	Line* addline = new Line(x1, y1, x2, y2);
	
	if (choice == 2)
		addline->bresDraw(PixelBuffer, width);

	else if (choice == 3)
		addline->ddaDraw(PixelBuffer, width);

	lines.insert(lines.end(), addline);
	numLines++;
}

void PolyDraw()
{
	int nv;
	cout << endl;
	cout << "Enter number of vertices: ";
	cin >> nv;
	
	int* txs = new int[nv];
	int* tys = new int[nv];
	
	for(int i = 0; i < nv; i++)
	{
		cout << "Enter Vertex " << i + 1 << ": ";
		cin >> txs[i] >> tys[i];
	}
	
	Poly* addpoly = new Poly(nv, txs, tys);
	polygons.insert(polygons.end(), addpoly);
	numPoly++;
	
	addpoly->rasterize(PixelBuffer, width, height);
	
	delete txs;
	delete tys;
}

void PolyTFs()
{
	int pindex, ch; // index of polygon working with
	
	cout << "Enter Polygon index number: ";
	cin >> pindex;
	
	if(pindex > numPoly)
	{
		cout << "Index out of range" << endl;
		return;
	}
	
	cout << "1. Translate" << endl << "2. Rotate" << endl<<  "3. Scale" << endl;
	cout << "Enter choice: ";
	cin >> ch;
	
	if(ch == 1) //translation
	{
		int tx, ty;
		cout << "Enter a translation vector tx ty: ";
		cin >> tx >> ty;
		polygons[pindex]->translate(tx, ty);
		redrawAll();
	}
	
	else if(ch == 2) // rotation
	{
		double deg;
		cout << "Enter angle in degrees: " << endl;
		cin >> deg;
		polygons[pindex]->rotate(deg);
		redrawAll();
	}
	
	else if(ch == 3) // scaling
	{
		double alpha, beta;
		cout << "Enter x scaling factor: ";
		cin >> alpha;
		cout << "Enter y scaling factor: ";
		cin >> beta;
		polygons[pindex]->scale(alpha, beta);
		redrawAll();
	}
	
	else
		cout << "Invalid choice" << endl;
}

void ClipAll()
{
	
	int xmin, xmax, ymin, ymax;
	cout << "Enter xmin: ";
	cin >> xmin;
	cout << "Enter xmax: ";
	cin >> xmax;
	cout << "Enter ymin: ";
	cin >> ymin;
	cout << "Enter ymax: ";
	cin >> ymax;
	
	
	for(int i = 0; i < numPoly; i++)
	{
		if(!polygons[i]->isGone())
			polygons[i]->clip(xmin, xmax, ymin, ymax);
	}
	
	for(int i = 0; i < numLines; i++)
	{
		if(!lines[i]->isGone())
			lines[i]->clip(xmin, xmax, ymin, ymax);
	}
	
	redrawAll();
}


void SaveScene()
{
	vector<Poly*> outpolys; //polygons to be written to file
	int tempv = 0; // temporary number vertices
	
	for(int i = 0; i < numPoly; i++)
	{
		if (!polygons[i]->isGone())
		{
			outpolys.insert(outpolys.end(), polygons[i]);
			tempv++;
		}
	}
	
	
	ofstream outfile;
	string input = "";
	int currnumv;
	
	cout << "Enter output filename: ";
	cin.ignore();
	getline(cin, input);
	
	outfile.open(input.c_str());
	
	outfile << tempv << endl;
	
	for(int i = 0; i < tempv; i++)
	{
		currnumv = outpolys[i]->numVertex();
		outfile << currnumv << endl;
		for(int j = 0; j < currnumv; j++)
		{
			outfile << outpolys[i]->getx(j) << " " << outpolys[i]->gety(j);
			outfile << endl;
		}
		outfile << endl;
	}
	
	outfile.close();
}

void LoadScene()
{
	ifstream infile;
	string input = "";
	int currnumv;
	
	cout << "Enter input filename: ";
	cin.ignore();
	getline(cin, input);
	
	infile.open(input.c_str());
	if(!infile)
	{
		cout << "Invalid filename" << endl;
		return;
	}
	
	polygons.clear();
	infile >> numPoly;
	
	for(int i = 0; i < numPoly; i++)
	{
		infile >> currnumv;
		int* xs = new int[currnumv];
		int* ys = new int[currnumv];
		for(int j = 0; j < currnumv; j++)
		{
			infile >> xs[j] >> ys[j];
		}
		
		Poly* addpoly = new Poly(currnumv, xs, ys);
		polygons.insert(polygons.end(), addpoly);
		
		delete xs;
		delete ys;
	}
	
	infile.close();
	redrawAll();
}

void printmenu()
{
	cout << endl;
	cout << "Menu" << endl;
	cout << "1. Clear All Objects" << endl;
	cout << "2. Draw Bresenham line" << endl;
	cout << "3. Draw DDA Line" << endl;
	cout << "4. Draw a Polygon" << endl;
	cout << "5. Polygon Transformations" << endl;
	cout << "6. Clipping" << endl;
	cout << "7. Save Scene" << endl;
	cout << "8. Load Scene" << endl;
	cout << endl;
}

void menu(unsigned char key, int x, int y)
{
	if(key != 109 && key != 77)
		return;
	int choice;
	printmenu();
	cout << "Enter a Choice: ";
	cin >> choice;
	switch (choice)
	{
	case 1: removeAll();
		clearBuffer(PixelBuffer, height, width);
		break;
	case 2: LineDraw(2);
		break;
	case 3: LineDraw(3);
		break;
	case 4: PolyDraw();
		break;
	case 5: PolyTFs();
		break;
	case 6: ClipAll();
		break;
	case 7: SaveScene();
		break;
	case 8: LoadScene();
		break;
	
	default: cout << "Invalid Option" << endl;
		break;
	} 


	glutPostRedisplay();
}
