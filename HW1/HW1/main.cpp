#include <GL/glut.h>
#include "Helper.h"
#include "Line.h"
#include "Poly.h"
#include <iostream>
#include <vector>
#include "UI.h"

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
	
	int xs3[] = { 50, 75, 90, 120, 160 };
	int ys3[] = { 50, 150, 90, 130, 50 };

	Poly* test2 = new Poly(5, xs3, ys3);
	polygons.insert(polygons.end(), test2);
	test2->draw(PixelBuffer, width);
	test2->clip(40, 70, 10, 60);
	test2->rasterize(PixelBuffer, height, width);
	numPoly++;
	//test2.scale(2, 2);
	//test2.rotate(45);
	//test2.draw(PixelBuffer, 200);
	//test2.rasterize(PixelBuffer, 200, 200);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	//set window size to height, width
	glutInitWindowSize(height, width);
	//set window position
	glutInitWindowPosition(100, 100);

	//create and set main window title
	int MainWindow = glutCreateWindow("Hello Graphics!!");

	glutKeyboardFunc(menu);
	glClearColor(0, 0, 0, 0); //clears the buffer of OpenGL
						

	glutDisplayFunc(display);//sets display function

	glutMainLoop();//main display loop, will display until terminate
	return 0;
}

//main display loop, this function will be called again and again by OpenGL
void display()
{
	cout << "begin display" << endl;
	//Misc.
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//draws pixel on screen, width and height must match pixel buffer dimension
	glDrawPixels(height, width, GL_RGB, GL_FLOAT, PixelBuffer);

	//window refresh
	glEnd();
	glFlush();

	cout << "end display" << endl;
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

void LineDraw(int choice)
{
	int x1 = 0;
	int x2, y1, y2;
	cout << "Enter endpoints of line: x1 y1 x2 y2" << endl;
	cin >> x1 >> y1 >> x2 >> y2;

	cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
	cout << endl;
	Line* addline = new Line(x1, y1, x2, y2);
	
	if (choice == 2)
		addline->bresDraw(PixelBuffer, width);

	else if (choice == 3)
		addline->ddaDraw(PixelBuffer, width);

	lines.insert(lines.end(), addline);
	numLines++;
}

void menu(unsigned char key, int x, int y)
{
	int choice;
	cout << "Enter a Choice" << endl;
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

	default: cout << "default" << endl;
			break;
	} 


	glutPostRedisplay();
}
