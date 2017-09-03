#include "Poly.h"
#include "Line.h"
#include "Helper.h"
#include <math.h>
#include <iostream>

using namespace std;

Poly::Poly(int numVertex, int* xarray, int* yarray)
{
	numv = numVertex;
	for (int i = 0; i < numVertex; i++)
	{
		xvals.insert(xvals.end(), xarray[i]);
		yvals.insert(yvals.end(), yarray[i]);
	}
	
	gone = false;
	setmaximas();
	calcCentroid();
}

void Poly::draw(float* buffer, int width)
{
	for (int i = 0; i < numv-1; i++)
	{
		Line temp(xvals[i], yvals[i], xvals[i+1], yvals[i+1]);
		temp.ddaDraw(buffer, width);
	}

	Line temp(xvals[numv-1], yvals[numv-1], xvals[0], yvals[0]);
	temp.ddaDraw(buffer, width);
}


void Poly::rasterize(float* buffer, int width, int height)
{
	float* tbuffer = new float[height * width * 3]; // create a temporary buffer to work with
	clearBuffer(tbuffer, height, width);

	for (int i = 0; i < numv - 1; i++)
	{
		Line temp(xvals[i], yvals[i], xvals[i + 1], yvals[i + 1]);
		if (!temp.horiz())
			temp.ddaDraw(tbuffer, width);
		else
			temp.plot(tbuffer, width);
	}

	Line temp(xvals[numv - 1], yvals[numv - 1], xvals[0], yvals[0]);
	if (!temp.horiz())
		temp.ddaDraw(tbuffer, width);
	else
		temp.plot(tbuffer, width);

	int xloc, xback;
	int yloc;
	bool in = false;
	int vindex = 0; // vertex index

	for (yloc = 0; yloc < height; yloc++)
	{
		in = false; //starting a new row in set to false
		for (xloc = 0; xloc < width - 1; xloc++)
		{
			if (checkPix(xloc, yloc, tbuffer, width)) // if current pixel is on
			{
				if (checkPix(xloc + 1, yloc, tbuffer, width)) //check the next pixel too in case the edge is shallow
				{
					vindex = findVertex(xloc, yloc);
					if (vindex != -1) // vertex found
					{
						if (maximas[vindex])
						{
							xback = xloc;
							in = !in; // since next pixel is on also, this loop will repeat, flipping once is equivalent to flipping twice
						}
					}
					
					else if (findVertex(xloc+1, yloc) == -1)
					{
						if(diffEdgePoint(xloc, yloc, xloc+1, yloc))
						{
							xback = xloc;
							in = !in;
						}
					}

					else // vertex not found and next pixel is on, same edge
						continue;
				}

				else // next pixel is not on
				{
					vindex = findVertex(xloc, yloc);
					if (vindex != -1) // vertex found
					{
						if (!maximas[vindex])
						{
							xback = xloc;
							in = !in; // flip the parity if the next pixel is not on and this is not a local maxima
						}
					}

					else // vertex not found flip parity
					{
						xback = xloc;
						in = !in;
					}
						
				}
			}

			else // current pixel is not on
			{
				if (in)
					MakePix(xloc, yloc, tbuffer, width);
			}
		}
		if (in == true)
		{
			for(int k = xloc; k > xback; k--)
				turnOffPix(k, yloc, tbuffer, width);
		}
	}

	comBuffer(buffer, tbuffer, height, width); //now put the temporary buffer into the actual buffer

	delete tbuffer;
}

int Poly::findVertex(int x, int y) // returns the location index or -1 if not found
{
	for (int i = 0; i < numv; i++)
		if (xvals[i] == x && yvals[i] == y)
			return i;

	return -1; // was not found if you get to this point
}

void Poly::setmaximas()
{
	maximas.clear(); //since this appends maximas, need to clear it first

	if ((yvals[numv - 1] > yvals[0] && yvals[1] > yvals[0]) || (yvals[numv - 1] < yvals[0] && yvals[1] < yvals[0])) // if first vertex is local maxima
		maximas.insert(maximas.end(), true);
	else
		maximas.insert(maximas.end(), false);


	for (int i = 1; i < numv - 1; i++)
	{
		if ((yvals[i - 1] > yvals[i] && yvals[i + 1] > yvals[i]) || (yvals[i - 1] < yvals[i] && yvals[i + 1] < yvals[i])) // if vertex i is a local maxima
			maximas.insert(maximas.end(), true);
		else
			maximas.insert(maximas.end(), false);
	}

	if ((yvals[0] > yvals[numv - 1] && yvals[numv - 2] > yvals[numv - 1]) || (yvals[0] < yvals[numv - 1] && yvals[numv - 2] < yvals[numv - 1])) // if last vertex is local maxima
		maximas.insert(maximas.end(), true);
	else
		maximas.insert(maximas.end(), false);

}

void Poly::calcCentroid() //calculate the centroid
{
	double tempx = 0;
	double tempy = 0;

	for (int i = 0; i < numv; i++) // sum the coordinates
	{
		tempx += xvals[i];
		tempy += yvals[i];
	}

	tempx = tempx / numv;
	tempy = tempy / numv;

	centx = int(round(tempx)); // divide by num of vertices to get average
	centy = int(round(tempy));
}

void Poly::translate(int tx, int ty) //move the vertices by vector tx, ty
{
	for (int i = 0; i < numv; i++)
	{
		xvals[i] = xvals[i] + tx;
		yvals[i] = yvals[i] + ty;
	}
}

void Poly::rotate(double deg) // takes degrees, rotate the vertices
{
	double pi = 3.14159265;
	double rad = deg * pi / 180;
	int tx, ty; // temp values for calculating when rotating

	translate(-centx, -centy);

	for (int i = 0; i < numv; i++)
	{
		tx = xvals[i];
		ty = yvals[i];
		xvals[i] = int(round(tx * cos(rad) - sin(rad) * ty));
		yvals[i] = int(round(tx * sin(rad) + cos(rad) * ty));
	}

	translate(centx, centy);
	setmaximas(); // new rotated coordinates may have different maximas
}

void Poly::scale(double alpha, double beta) // scales the polygon
{
	translate(-centx, -centy);
	for (int i = 0; i < numv; i++)
	{
		xvals[i] = int(round(xvals[i] * alpha));
		yvals[i] = int(round(yvals[i] * beta));
	}

	translate(centx, centy);
}

bool Poly::diffEdgePoint(int x1, int y1, int x2, int y2)
{
	for(int i = 0; i < numv - 1; i++)
	{
		Line tmp(xvals[i], yvals[i], xvals[i+1], yvals[i+1]);
		if (tmp.diffEdge(x1, y1, x2, y2))
			return true;
	}
	
	Line tmp(xvals[numv-1], yvals[numv-1], xvals[0], yvals[0]);
		if (tmp.diffEdge(x1, y1, x2, y2))
			return true;
			
	return false;
}

void Poly::clip(int xmin, int xmax, int ymin, int ymax)
{
	vector<int> tx1, tx2, ty1, ty2; // temporary vectors for clipping
	int tmx, tmy; //temp variables
	
	for(int i = 0; i < numv-1; i++) //clip left
	{
		if(xvals[i] < xmin && xvals[i+1] >= xmin) // out -> in intersect and p2
		{
			Line temp(xvals[i], yvals[i], xvals[i+1], yvals[i+1]);
			temp.vertsect(xmin, tmx, tmy);
			tx1.insert(tx1.end(), tmx);
			ty1.insert(ty1.end(), tmy);
			tx1.insert(tx1.end(), xvals[i+1]);
			ty1.insert(ty1.end(), yvals[i+1]);
		}
		
		else if (xvals[i] >= xmin && xvals[i+1] >= xmin) // in -> in p2
		{
			tx1.insert(tx1.end(), xvals[i+1]);
			ty1.insert(ty1.end(), yvals[i+1]);
		}
		
		else if (xvals[i] >= xmin && xvals[i+1] < xmin) // in -> out intersect
		{
			Line temp(xvals[i], yvals[i], xvals[i+1], yvals[i+1]);
			temp.vertsect(xmin, tmx, tmy);
			tx1.insert(tx1.end(), tmx);
			ty1.insert(ty1.end(), tmy);
		}
		
		//out -> out do nothing
	}
	
	if(xvals[numv-1] < xmin && xvals[0] >= xmin) // out -> in intersect p2
	{
		Line temp(xvals[numv-1], yvals[numv-1], xvals[0], yvals[0]);
		temp.vertsect(xmin, tmx, tmy);
		tx1.insert(tx1.end(), tmx);
		ty1.insert(ty1.end(), tmy);
		tx1.insert(tx1.end(), xvals[0]);
		ty1.insert(ty1.end(), yvals[0]);
	}
		
	else if (xvals[numv-1] >= xmin && xvals[0] >= xmin) // in -> in p2
	{
		tx1.insert(tx1.end(), xvals[0]);
		ty1.insert(ty1.end(), yvals[0]);
	}
		
	else if (xvals[numv-1] >= xmin && xvals[0] < xmin) // in -> out intersect
	{
		Line temp(xvals[numv-1], yvals[numv-1], xvals[0], yvals[0]);
		temp.vertsect(xmin, tmx, tmy);
		tx1.insert(tx1.end(), tmx);
		ty1.insert(ty1.end(), tmy);
	}
	
	
	// out -> out do nothing
	
	int cnumv = tx1.size();
	if(cnumv == 0)
	{
		gone = true;
		return;
	}
	
	
	for(int i = 0; i < cnumv-1; i++) //clip right
	{
		if(tx1[i] > xmax && tx1[i+1] <= xmax) // out -> in intersect and p2
		{
			Line temp(tx1[i], ty1[i], tx1[i+1], ty1[i+1]);
			temp.vertsect(xmax, tmx, tmy);
			tx2.insert(tx2.end(), tmx);
			ty2.insert(ty2.end(), tmy);
			tx2.insert(tx2.end(), tx1[i+1]);
			ty2.insert(ty2.end(), ty1[i+1]);
		}
		
		else if (tx1[i] <= xmax && tx1[i+1] <= xmax) // in -> in p2
		{
			tx2.insert(tx2.end(), tx1[i+1]);
			ty2.insert(ty2.end(), ty1[i+1]);
		}
		
		else if (tx1[i] <= xmax && tx1[i+1] > xmax) // in -> out intersect
		{
			Line temp(tx1[i], ty1[i], tx1[i+1], ty1[i+1]);
			temp.vertsect(xmax, tmx, tmy);
			tx2.insert(tx2.end(), tmx);
			ty2.insert(ty2.end(), tmy);
		}
		
		//out -> out do nothing
	}
	
	if(tx1[cnumv-1] > xmax && tx1[0] <= xmax) // out -> in intersect p2
	{
		Line temp(tx1[cnumv-1], ty1[cnumv-1], tx1[0], ty1[0]);
		temp.vertsect(xmax, tmx, tmy);
		tx2.insert(tx2.end(), tmx);
		ty2.insert(ty2.end(), tmy);
		tx2.insert(tx2.end(), tx1[0]);
		ty2.insert(ty2.end(), ty1[0]);
	}
		
	else if (tx1[cnumv-1] <= xmax && tx1[0] <= xmax) // in -> in p2
	{
		tx2.insert(tx2.end(), tx1[0]);
		ty2.insert(ty2.end(), ty1[0]);
	}
		
	else if (tx1[cnumv-1] <= xmax && tx1[0] > xmax) // in -> out intersect
	{
		Line temp(tx1[cnumv-1], ty1[cnumv-1], tx1[0], ty1[0]);
		temp.vertsect(xmax, tmx, tmy);
		tx2.insert(tx2.end(), tmx);
		ty2.insert(ty2.end(), tmy);
	}
	//out -> out do nothing
	
	cnumv = tx2.size();
	if(cnumv == 0)
	{
		gone = true;
		return;
	}
	
	tx1.clear(); // clear tx1 and ty1, now putting into tx1 and ty1
	ty1.clear();
	
	
	for(int i = 0; i < cnumv - 1; i++) // clip below
	{
		if(ty2[i] < ymin && ty2[i+1] >= ymin) // out -> in intersect p2
		{
			Line temp(tx2[i], ty2[i], tx2[i+1], ty2[i+1]);
			temp.horzsect(ymin, tmx, tmy);
			tx1.insert(tx1.end(), tmx);
			ty1.insert(ty1.end(), tmy);
			tx1.insert(tx1.end(), tx2[i+1]);
			ty1.insert(ty1.end(), ty2[i+1]);
		}
		
		else if(ty2[i] >= ymin && ty2[i+1] >= ymin) //in -> in
		{
			tx1.insert(tx1.end(), tx2[i+1]);
			ty1.insert(ty1.end(), ty2[i+1]);
		}
		
		else if(ty2[i] >= ymin && ty2[i+1] < ymin) // in -> out intersect 
		{
			Line temp(tx2[i], ty2[i], tx2[i+1], ty2[i+1]);
			temp.horzsect(ymin, tmx, tmy);
			tx1.insert(tx1.end(), tmx);
			ty1.insert(ty1.end(), tmy);
		}
		// out -> out nothing
	}
	
	if(ty2[cnumv-1] < ymin && ty2[0] >= ymin) // out -> in intersect p2
	{
		Line temp(tx2[cnumv-1], ty2[cnumv-1], tx2[0], ty2[0]);
		temp.horzsect(ymin, tmx, tmy);
		tx1.insert(tx1.end(), tmx);
		ty1.insert(ty1.end(), tmy);
		tx1.insert(tx1.end(), tx2[0]);
		ty1.insert(ty1.end(), ty2[0]);
	}
		
	else if(ty2[cnumv-1] >= ymin && ty2[0] >= ymin) //in -> in
	{
		tx1.insert(tx1.end(), tx2[0]);
		ty1.insert(ty1.end(), ty2[0]);
	}
		
	else if(ty2[cnumv-1] >= ymin && ty2[0] < ymin) // in -> out intersect 
	{
		Line temp(tx2[cnumv-1], ty2[cnumv-1], tx2[0], ty2[0]);
		temp.horzsect(ymin, tmx, tmy);
		tx1.insert(tx1.end(), tmx);
		ty1.insert(ty1.end(), tmy);
	}
	//out -> out nothing
	
	cnumv = tx1.size();
	if(cnumv == 0)
	{
		gone = true;
		return;
	}
	
	
	tx2.clear(); //inserting into tx2 ty2, must clear
	ty2.clear();
	
	

	for(int i = 0; i < cnumv - 1; i++) // clip above
	{
		if(ty1[i] > ymax && ty1[i+1] <= ymax) // out -> in intersect p2
		{
			Line temp(tx1[i], ty1[i], tx1[i+1], ty1[i+1]);
			temp.horzsect(ymax, tmx, tmy);
			tx2.insert(tx2.end(), tmx);
			ty2.insert(ty2.end(), tmy);
			tx2.insert(tx2.end(), tx1[i+1]);
			ty2.insert(ty2.end(), ty1[i+1]);
		}
		
		else if(ty1[i] <= ymax && ty1[i+1] <= ymax) //in -> in
		{
			tx2.insert(tx2.end(), tx1[i+1]);
			ty2.insert(ty2.end(), ty1[i+1]);
		}
		
		else if(ty1[i] <= ymax && ty1[i+1] > ymax) // in -> out intersect
		{
			Line temp(tx1[i], ty1[i], tx1[i+1], ty1[i+1]);
			temp.horzsect(ymax, tmx, tmy);
			tx2.insert(tx2.end(), tmx);
			ty2.insert(ty2.end(), tmy);
		}
		// out -> out nothing
	}
	
	if(ty1[cnumv-1] > ymax && ty1[0] <= ymax) // out -> in intersect p2
	{
		Line temp(tx1[cnumv-1], ty1[cnumv-1], tx1[0], ty1[0]);
		temp.horzsect(ymax, tmx, tmy);
		tx2.insert(tx2.end(), tmx);
		ty2.insert(ty2.end(), tmy);
		tx2.insert(tx2.end(), tx1[0]);
		ty2.insert(ty2.end(), ty1[0]);
	}
		
	else if(ty1[cnumv-1] <= ymax && ty1[0] <= ymax) //in -> in
	{
		tx2.insert(tx2.end(), tx1[0]);
		ty2.insert(ty2.end(), ty1[0]);
	}
		
	else if(ty1[cnumv-1] <= ymax && ty1[0] > ymax) // in -> out intersect
	{
		Line temp(tx1[cnumv-1], ty1[cnumv-1], tx1[0], ty1[0]);
		temp.horzsect(ymax, tmx, tmy);
		tx2.insert(tx2.end(), tmx);
		ty2.insert(ty2.end(), tmy);
	}
	
	numv = tx2.size();
	xvals = tx2;
	yvals = ty2;
	
	setmaximas();
	calcCentroid();
	
	
}


bool Poly::isGone()
{
	return gone;
}
