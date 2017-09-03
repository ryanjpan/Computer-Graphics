#include "Line.h"
#include "Helper.h"
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

Line::Line(int lx, int ly, int rx, int ry) // Line automatically swaps points if necessary so always leftx < rightx
{
	if (lx > rx)
	{
		leftx = rx;
		lefty = ry;
		rightx = lx;
		righty = ly;
	}

	else
	{
		leftx = lx;
		lefty = ly;
		rightx = rx;
		righty = ry;
	}

	gone = false;
}

void Line::ddaDraw(float* buffer, int width) // dda algorithm
{
	MakePix(leftx, lefty, buffer, width); //turn on left endpoint
	if (rightx == leftx) // vertical line cannot calculate m
	{
		int tempy;
		if (lefty > righty)
			tempy = righty;
		else
			tempy = lefty;

		for (int i = 0; i <= abs(righty - lefty); i++)
			MakePix(leftx, tempy + i, buffer, width);
	}

	double m = double((righty - lefty)) / (rightx - leftx);
	double mbar = 1 / m;
	double x = leftx;
	double y = lefty;
	int roundedy = 0;
	int roundedx = 0;

	if (m == 0) // horizontal line
	{
		for (int i = 1; i <= rightx - leftx; i++)
			MakePix(int(x + i), int(y), buffer, width);
	}

	else if (m >= -1 && m <= 1) //shallow positive slope
	{
		for (int i = 1; i <= rightx - leftx; i++)
		{
			y = y + m;
			roundedy = int(round(y));
			MakePix(int(x + i), roundedy, buffer, width);
		}
	}

	else if (m > 1) //steep postive slope
	{
		for (int i = 1; i <= righty - lefty; i++)
		{
			x = x + mbar;
			roundedx = int(round(x));
			MakePix(roundedx, int(y + i), buffer, width);
		}
	}

	else // negative steep slope
	{
		mbar = -mbar; // x still needs to go to the right, flip the sign
		for (int i = 1; i <= lefty - righty; i++)
		{
			x = x + mbar;
			roundedx = int(round(x));
			MakePix(roundedx, int(y - i), buffer, width);
		}
	}
}

void Line::bresDraw(float* buffer, int width) //bresenham algorithm
{
	if (gone)
		return;

	int dy = righty - lefty;
	int dx = rightx - leftx;
	int y = lefty;
	int x = leftx;
	int nexty = lefty;
	int nextx = leftx;
	int Pi = 0;

	if (dx == 0) // vertical line, since calculate m later, do not want to divide by zero
	{
		int tempy;
		if (lefty > righty)
			tempy = righty;
		else
			tempy = lefty;

		if (dy < 0) // dy must be positive or bresenham gives incorrect vertical line
			dy = -dy;

		Pi = (2 * dx) - dy;
		for (int i = 0; i <= abs(righty - lefty); i++)
		{
			if (Pi >= 0)
				nextx = x + 1;
			Pi = Pi + (2 * dx) - (2 * dy) * (nextx - x);
			MakePix(x, tempy + i, buffer, width);
			x = nextx;
		}

		return;
	}

	double m = double(dy) / dx; //if not vertical line check which case


	if (m >= 0 && m <= 1) // shallow positive
	{
		Pi = (2 * dy) - dx;
		for (int i = 0; i <= rightx - leftx; i++)
		{
			if (Pi >= 0)
				nexty = y + 1;
			Pi = Pi + (2 * dy) - (2 * dx) * (nexty - y);
			MakePix(x + i, y, buffer, width);
			y = nexty;
		}
	}

	else if (m >= -1 && m < 0) // shallow negative
	{
		Pi = (-2 * dy) - dx;
		for (int i = 0; i <= rightx - leftx; i++)
		{
			if (Pi >= 0)
				nexty = y - 1;
			Pi = Pi + (2 * dx) * (nexty - y) - (2 * dy);
			MakePix(x + i, y, buffer, width);
			y = nexty;
		}
	}

	else if (m > 1) // steep positive
	{
		Pi = (2 * dx) - dy;
		for (int i = 0; i <= righty - lefty; i++)
		{
			if (Pi >= 0)
				nextx = x + 1;
			Pi = Pi + (2 * dx) - (2 * dy) * (nextx - x);
			MakePix(x, y + i, buffer, width);
			x = nextx;
		}
	}

	else // m < -1 steep negative
	{
		Pi = -dy - (2 * dx);
		for (int i = 0; i <= lefty - righty; i++)
		{
			if (Pi < 0)
				nextx = x + 1;
			Pi = Pi - (2 * dx) - 2 * dy * (nextx - x);
			MakePix(x, y - i, buffer, width);
			x = nextx;
		}
	}
}

bool Line::horiz() //checks if the line is horizontal
{
	if (lefty == righty)
		return true;

	else
		return false;
}

void Line::plot(float* buffer, int width) // only plots the end points of the line
{
	MakePix(leftx, lefty, buffer, width);
	MakePix(rightx, righty, buffer, width);
}

void Line::clip(int xmin, int xmax, int ymin, int ymax)
{
	int leftcode = 0; // code for left endpoint
	int rightcode = 0; // code for right endpoint

	leftcode += sbit(leftx - xmin); // handling 000x bit position for code
	rightcode += sbit(rightx - xmin);

	leftcode += sbit(xmax - leftx) * 2; // handling 00x0 bit position for code
	rightcode += sbit(xmax - rightx) * 2; 

	leftcode += sbit(lefty - ymin) * 4; // handling 0x00 bit position for code
	rightcode += sbit(righty - ymin) * 4;

	leftcode += sbit(ymax - lefty) * 8; // handling x000 bit position for code
	rightcode += sbit(ymax - righty) * 8;

	cout << "leftcode: " << leftcode << endl;
	cout << "rightcode: " << rightcode << endl;
	

	if (leftcode == 0 && rightcode == 0) // line is within viewport, we're done
		return;

	if (leftcode & rightcode) // line is completely outside viewport, eliminate and done
	{
		gone = true;
		return;
	}

	int x, y; // new xs and ys

	if (leftx == rightx) // vertical line handle separately
	{
		if (leftcode & 8) // if above just set y value to ymax
			lefty = ymax;

		if (rightcode & 8) // same as above if right endpoint above
			righty = ymax;

		if (leftcode & 4) // if below set y value to ymin
			lefty = ymin;

		if (rightcode & 4) // same as above if right endpoint below
			righty = ymin;

		return;
	}

	if (lefty == righty) // horizontal line
	{
		if (leftcode & 1)
			leftx = xmin;

		if (rightcode & 2)
			rightx = xmax;

		return;
	}

	double m = double(righty - lefty) / (rightx - leftx);


	if (leftcode & 1) //clip left, since line class has leftx < lefty, it must be the left point that is being clipped
	{
		y = int(round(lefty + m * (xmin - leftx)));
		leftx = xmin;
		lefty = y;
	}

	if (rightcode & 2) //clip right, same reason above, right side must be clipped for xmax
	{
		y = int(round(lefty + m * (xmax - leftx)));
		rightx = xmax;
		righty = y;
	}

	if (leftcode & 4) // clip below if left point
	{
		x = int(round(leftx + (ymin - lefty) / m));
		leftx = x;
		lefty = ymin;
	}

	if (rightcode & 4) // clip below if right point
	{
		x = int(round(leftx + (ymin - lefty) / m));
		rightx = x;
		righty = ymin;
	}

	if (leftcode & 8) // clip above if left point
	{
		x = int(round(leftx + (ymax - lefty) / m));
		leftx = x;
		lefty = ymax;
	}

	if (rightcode & 8) // clip above if right point
	{
		x = int(round(leftx + (ymax - lefty) / m));
		rightx = x;
		righty = ymax;
	}

}

bool Line::isGone()
{
	return gone;
}

bool Line::diffEdge(int x1, int y1, int x2, int y2)
{
	bool t1, t2;
	t1 = isOnLine(x1, y1);
	t2 = isOnLine(x2, y2);
	if(t1 && !t2)
		return true;
	
	if(!t1 && t2)
		return true;
		
	return false;
}

void Line::horzsect(int yval, int& xint, int& yint)
{
	if(leftx == rightx) // vertical line
	{
		xint = leftx;
		yint = yval;
	}
	double m = double(righty - lefty) / (rightx - leftx);
	
	int x = round(leftx + double(yval - lefty) / m);
	xint = x;
	yint = yval;
}
void Line::vertsect(int xval, int& xint, int& yint)
{
	if(lefty == righty) // horizontal line
	{
		xint = xval;
		yint = lefty;
	}
	double m = double(righty - lefty) / (rightx - leftx);
	
	int y = round(m* (xval - leftx) + lefty);
	xint = xval;
	yint = y;
}

bool Line::isOnLine(int cx, int cy)
{
	vector<int> tx;
	vector<int> ty;
	
	tx.insert(tx.begin(), leftx);
	ty.insert(ty.begin(), lefty);
	
	if (rightx == leftx) // vertical line cannot calculate m
	{
		int tempy;
		if (lefty > righty)
			tempy = righty;
		else
			tempy = lefty;

		for (int i = 0; i <= abs(righty - lefty); i++)
		{
			tx.insert(tx.end(), leftx);
			ty.insert(ty.end(), tempy + i);	
		}
	}

	double m = double((righty - lefty)) / (rightx - leftx);
	double mbar = 1 / m;
	double x = leftx;
	double y = lefty;
	int roundedy = 0;
	int roundedx = 0;

	if (m == 0) // horizontal line
	{
		for (int i = 1; i <= rightx - leftx; i++)
		{
			tx.insert(tx.end(), leftx+i);
			ty.insert(ty.end(), y);
		}
	}

	else if (m >= -1 && m <= 1) //shallow positive slope
	{
		for (int i = 1; i <= rightx - leftx; i++)
		{
			y = y + m;
			roundedy = round(y);
			
			tx.insert(tx.end(), int(x+i));
			ty.insert(ty.end(), roundedy);
		}
	}

	else if (m > 1) //steep postive slope
	{
		for (int i = 1; i <= righty - lefty; i++)
		{
			x = x + mbar;
			roundedx = round(x);
			
			tx.insert(tx.end(), roundedx);
			ty.insert(ty.end(), int(y+i));
		}
	}

	else // negative steep slope
	{
		mbar = -mbar; // x still needs to go to the right, flip the sign
		for (int i = 1; i <= lefty - righty; i++)
		{
			x = x + mbar;
			roundedx = int(round(x));
			
			tx.insert(tx.end(), roundedx);
			ty.insert(ty.end(), int(y - i));
		}
	}
	
	for(int i = 0; i < int(tx.size()); i++)
		if(cx == tx[i] && cy == ty[i])
			return true;
	
	return false;
}
