#include "Poly.h"
#include "Line.h"
#include "Helper.h"
#include <math.h>
#include <iostream>

using namespace std;

Poly::Poly(int numVertex, float* xarray, float* yarray, float* zarray)
{
	vector<int> temp;
	numv = numVertex;
	for (int i = 0; i < numVertex; i++)
	{
		xvals.insert(xvals.end(), xarray[i]);
		yvals.insert(yvals.end(), yarray[i]);
		zvals.insert(zvals.end(), zarray[i]);
		adj.insert(adj.end(), temp); // need to push empty vectors into vector
	}

	calcCentroid();
}

void Poly::calcCentroid() //calculate the centroid
{
	float tempx = 0;
	float tempy = 0;
	float tempz = 0;

	for (int i = 0; i < numv; i++) // sum the coordinates
	{
		tempx += xvals[i];
		tempy += yvals[i];
		tempz += zvals[i];
	}

	tempx = tempx / numv;
	tempy = tempy / numv;
	tempz = tempz / numv;

	centx = tempx; // divide by num of vertices to get average
	centy = tempy;
	centz = tempz;
}

void Poly::translate(float tx, float ty, float tz) //move the vertices by vector tx, ty, tz
{
	for (int i = 0; i < numv; i++)
	{
		xvals[i] = xvals[i] + tx;
		yvals[i] = yvals[i] + ty;
		zvals[i] = zvals[i] + tz;
	}

	centx += tx;
	centy += ty;
	centz += tz;
}

void Poly::rotate(double deg, float p1x, float p1y, float p1z, float p2x, float p2y, float p2z) // takes degrees, rotate the vertices
{
	float dx = p2x - p1x;
	float dy = p2y - p1y;
	float dz = p2z - p1z;
	float mag = sqrt(dx * dx + dy * dy + dz * dz);
	dx = dx / mag;
	dy = dy / mag;
	dz = dz / mag;
	double pi = 3.14159265;
	double rad = deg * pi / 180;
	float tx, ty, tz, px, py, pz; // two sets of temp variables
	float l = sqrt(dy * dy + dz * dz);
	
	if(dy == 0 && dz == 0) // this means rotation about x axis. will get error if not manually handled
	{
		translate(-p1x, -p1y, -p1z);
		for(int i = 0; i < numv; i++)
		{
			tx = xvals[i];
			ty = yvals[i];
			tz = zvals[i];
			
			yvals[i] = cos(rad) * ty - sin(rad) * tz;
			zvals[i] = sin(rad) * ty + cos(rad) * tz;
		}
		translate(p1x, p1y, p1z);
		calcCentroid();
		return;
	}
	
	translate(-p1x, -p1y, -p1z); // M1
	
	for(int i = 0; i < numv; i++) // M2
	{
		tx = xvals[i];
		ty = yvals[i];
		tz = zvals[i];
		
		px = tx;
		py = dz/l * ty - dy/l * tz;
		pz = dy/l * ty + dz/l * tz;
		
		xvals[i] = l * px - dx * pz;
		yvals[i] = py;
		zvals[i] = dx * px + l * pz;
	}
	
	for(int i = 0; i < numv; i++) // M3
	{
		tx = xvals[i];
		ty = yvals[i];
		tz = zvals[i];
		
		xvals[i] = cos(rad) * tx - sin(rad) * ty;
		yvals[i] = sin(rad) * tx + cos(rad) * ty;
		//z unchanged
	}
	
	for(int i = 0; i < numv; i++) // M4
	{
		tx = xvals[i];
		ty = yvals[i];
		tz = zvals[i];
		
		px = l * tx + dx * tz;
		py = ty;
		pz = -dx * tx + l * tz;
		
		xvals[i] = px;
		yvals[i] = dz/l * py + dy/l * pz;
		zvals[i] = -dy/l * py + dz/l * pz;
	}
	
	translate(p1x, p1y, p1z); // M5
	
	calcCentroid();
	
}

void Poly::scale(float alpha, float beta, float gamma) // scales the polygon
{
	float tmpx = centx;
	float tmpy = centy;
	float tmpz = centz;

	translate(-tmpx, -tmpy, -tmpz);

	for (int i = 0; i < numv; i++)
	{
		xvals[i] = xvals[i] * alpha;
		yvals[i] = yvals[i] * beta;
		zvals[i] = zvals[i] * gamma;
	}

	translate(tmpx, tmpy, tmpz);
}


int Poly::getNumVertex()
{
	return numv;
}

float Poly::getx(int index)
{
	return xvals[index];
}

float Poly::gety(int index)
{
	return yvals[index];
}

float Poly::getz(int index)
{
	return zvals[index];
}

void Poly::getMinMax(float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax) // returns the smallest and largest number in each dimension
{
	xmin = xmax = xvals[0]; // initialize both min and max of each dimension to the first vertex
	ymin = ymax = yvals[0];
	zmin = zmax = zvals[0];
	for (int i = 1; i < numv; i++)
	{
		if (xvals[i] < xmin)
			xmin = xvals[i];
		else if (xvals[i] > xmax)
			xmax = xvals[i];

		if (yvals[i] < ymin)
			ymin = yvals[i];
		else if (yvals[i] > ymax)
			ymax = yvals[i];

		if (zvals[i] < zmin)
			zmin = zvals[i];
		else if (zvals[i] > zmax)
			zmax = zvals[i];
	}
}

void Poly::projectXY(float* buffer, int height, int width, float mdelta, float gminx, float gminy)
{
	float tx1, ty1, tx2, ty2;
	int ind;

	for (int i = 0; i < numv; i++) // draw all the lines using the adj list
	{
		int numadj = adj[i].size();
		for (int j = 0; j < numadj; j++)
		{
			ind = adj[i][j]; //index from adjacency list
			tx1 = xvals[i];
			tx2 = xvals[ind];
			ty1 = yvals[i];
			ty2 = yvals[ind];
			ndc(tx1, ty1, gminx, gminy, mdelta, width);
			ndc(tx2, ty2, gminx, gminy, mdelta, width);

			Line temp(round(tx1), round(ty1), round(tx2), round(ty2));
			temp.ddaDraw(buffer, width);

		}
	}
	
}

void Poly::projectYZ(float* buffer, int height, int width, float mdelta, float gminy, float gminz)
{
	float tz1, ty1, tz2, ty2;
	int ind;

	for (int i = 0; i < numv; i++) // draw all the lines using the adj list
	{
		int numadj = adj[i].size();
		for (int j = 0; j < numadj; j++)
		{
			ind = adj[i][j]; //index from adjacency list
			tz1 = zvals[i];
			tz2 = zvals[ind];
			ty1 = yvals[i];
			ty2 = yvals[ind];
			ndc(ty1, tz1, gminy, gminz, mdelta, width);
			ndc(ty2, tz2, gminy, gminz, mdelta, width);

			Line temp(round(ty1), round(tz1), round(ty2), round(tz2));;
			temp.ddaDraw(buffer, width);

		}
	}
}

void Poly::projectXZ(float* buffer, int height, int width, float mdelta, float gminx, float gminz)
{
	float tx1, tz1, tx2, tz2;
	int ind;

	for (int i = 0; i < numv; i++) // draw all the lines using the adj list
	{
		int numadj = adj[i].size();
		for (int j = 0; j < numadj; j++)
		{
			ind = adj[i][j]; //index from adjacency list
			tx1 = xvals[i];
			tx2 = xvals[ind];
			tz1 = zvals[i];
			tz2 = zvals[ind];
			ndc(tx1, tz1, gminx, gminz, mdelta, width);
			ndc(tx2, tz2, gminx, gminz, mdelta, width);

			Line temp(round(tx1), round(tz1), round(tx2), round(tz2));
			temp.ddaDraw(buffer, width);

		}
	}
}

void Poly::ndc(float& d1, float& d2, float min1, float min2, float mdelta, int N) //normalize the coordinates
{
	float nd1, nd2;

	nd1 = float(d1 - min1) / mdelta;
	nd2 = float(d2 - min2) / mdelta;

	d1 = nd1 * (N - 1);
	d2 = nd2 * (N - 1);
}

void Poly::setAdj(int ind, int newAdj)
{
	adj[ind].insert(adj[ind].end(), newAdj);
}

void Poly::setNumAdj(int num)
{
	numAdj = num;
}

int Poly::getNumAdj()
{
	return numAdj;
}

void Poly::printAdj(ostream& outfile)
{
	int currnumadj;
	for (int i = 0; i < numv; i++)
	{
		currnumadj = adj[i].size();
		for (int j = 0; j < currnumadj; j++)
		{
			outfile << i + 1 << " " << adj[i][j] + 1 << endl;
		}
	}
}

void Poly::showCoord()
{
	for(int i = 0; i < numv; i++)
	{
		cout << "Vertex " << i+1 << ": " << xvals[i] << " "
			<< yvals[i] << " " << zvals[i] << endl;
	}
}
