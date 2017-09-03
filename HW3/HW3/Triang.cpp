#include "Triang.h"
#include "Line.h"
#include "Helper.h"
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

Triang::Triang(float* xarray, float* yarray, float* zarray, int i1, int i2, int i3)
{
	for (int i = 0; i < 3; i++)
	{
		xvals[i] = xarray[i];
		yvals[i] = yarray[i];
		zvals[i] = zarray[i];
	}

	indices[0] = i1;
	indices[1] = i2;
	indices[2] = i3;
}

Triang::Triang()
{
	for (int i = 0; i < 3; i++)
	{
		xvals[i] = 0;
		yvals[i] = 0;
		zvals[i] = 0;
		nxs[i] = 0;
		nys[i] = 0;
		nzs[i] = 0;
		indices[i] = 0;
	}
}

int Triang::ndc(float val, float min, float mdelta, int N)
{
	int normval;

	normval = round((val - min) / mdelta * (N-1));

	return normval;
}

float Triang::getsx()
{
	float temp = 0;
	for(int i = 0; i < 3; i++)
		temp += xvals[i];
		
	return temp / 3;
	
	/*float smallest = xvals[0];

	if (xvals[1] < smallest)
		smallest = xvals[1];

	if (xvals[2] < smallest)
		smallest = xvals[2];

	return smallest;
	*/
}

float Triang::getsy()
{
	float temp = 0;
	for(int i = 0; i < 3; i++)
		temp += yvals[i];
		
	return temp / 3;
	
	/*float smallest = yvals[0];

	if (yvals[1] < smallest)
		smallest = yvals[1];

	if (yvals[2] < smallest)
		smallest = yvals[2];

	return smallest;
	*/
}

float Triang::getsz()
{
	float temp = 0;
	for(int i = 0; i < 3; i++)
		temp += zvals[i];
		
	return temp / 3;
	
	/*
	float smallest = zvals[0];

	if (zvals[1] < smallest)
		smallest = zvals[1];

	if (zvals[2] < smallest)
		smallest = zvals[2];

	return smallest;
	*/
}

void Triang::ndcAll(float mdelta, float gminx, float gminy, float gminz, int N)
{
	for (int i = 0; i < 3; i++)
	{
		nxs[i] = ndc(xvals[i], gminx, mdelta, N);
		nys[i] = ndc(yvals[i], gminy, mdelta, N);
		nzs[i] = ndc(zvals[i], gminz, mdelta, N);
	}
}

void Triang::rastXY(float* buffer, int height, int width, float* rs, float* gs, float* bs)
{
	float* tbuffer = new float[height * width * 3];
	float lcr, lcg, lcb, rcr, rcg, rcb;
	int j, left, right;

	clearBuffer(tbuffer, height, width);
	
	for (int i = 0; i < 2; i++)
	{
		Line edge(nxs[i], nys[i], nxs[i + 1], nys[i + 1]);
		edge.linInterpol(tbuffer, width, 1, 1, 1, 1, 1, 1);
		edge.linInterpol(buffer, width, rs[i], gs[i], bs[i], rs[i+1], gs[i+1], bs[i+1]);
	}

	Line edge(nxs[2], nys[2], nxs[0], nys[0]);
	edge.linInterpol(tbuffer, width, 1, 1, 1, 1, 1, 1);
	edge.linInterpol(buffer, width, rs[2], gs[2], bs[2], rs[0], gs[0], bs[0]);

	for (int i = 0; i < height; i++)
	{
		left = -1;
		right = -1;
		j = 0;
		while (j < width) // find first pixel on
		{
			if (!checkPix(j, i, tbuffer, width))
				j++;
			else
				break;
		}

		if (j == width) //no pixels on in this row
			continue;
		else
			left = j;

		j++;

		while (j < width) // look for the first pixel not on
		{
			if (checkPix(j, i, tbuffer, width))
			{
				left = j;
				j++;
			}
			else // first pixel not on
				break;
		}

		if (j >= width - 1) //no set of two points on this line
			continue;

		j++;
		
		while (j < width)
		{
			if (checkPix(j, i, tbuffer, width))
			{
				right = j;
				break;
			}
			else
				j++;
		}

		if (j == width) //no right endpoint found
			continue;
		
		Line temp(left, i, right, i);
		getColor(left, i, buffer, width, lcr, lcg, lcb);
		getColor(right, i, buffer, width, rcr, rcg, rcb);
		temp.linInterpol(buffer, width, lcr, lcg, lcb, rcr, rcg, rcb);
	}
	
	delete tbuffer;
}

void Triang::rastXZ(float* buffer, int height, int width, float* rs, float* gs, float* bs)
{
	float lcr, lcg, lcb, rcr, rcg, rcb;
	float* tbuffer = new float[height * width * 3];
	int j, left, right;

	clearBuffer(tbuffer, height, width);
	
	for (int i = 0; i < 2; i++)
	{
		Line edge(nxs[i], nzs[i], nxs[i + 1], nzs[i + 1]);
		edge.linInterpol(tbuffer, width, 1, 1, 1, 1, 1, 1);
		edge.linInterpol(buffer, width, rs[i], gs[i], bs[i], rs[i + 1], gs[i + 1], bs[i + 1]);
	}

	Line edge(nxs[2], nzs[2], nxs[0], nzs[0]);
	edge.linInterpol(tbuffer, width, 1, 1, 1, 1, 1, 1);
	edge.linInterpol(buffer, width, rs[2], gs[2], bs[2], rs[0], gs[0], bs[0]);

	for (int i = 0; i < height; i++)
	{
		left = -1;
		right = -1;
		j = 0;
		while (j < width)
		{
			if (!checkPix(j, i, tbuffer, width))
				j++;
			else
				break;
		}

		if (j == width) //no pixels on in this row
			continue;
		else
			left = j;

		j++;

		while (j < width) // look for the first pixel not on
		{
			if (checkPix(j, i, tbuffer, width))
			{
				left = j;
				j++;
			}
			else // first pixel not on
				break;
		}

		if (j >= width - 1) //no set of two points on this line
			continue;

		j++;

		while (j < width)
		{
			if (checkPix(j, i, tbuffer, width))
			{
				right = j;
				break;
			}
			else
				j++;
		}

		if (j == width) //no right endpoint found
			continue;

		Line temp(left, i, right, i);
		getColor(left, i, buffer, width, lcr, lcg, lcb);
		getColor(right, i, buffer, width, rcr, rcg, rcb);
		temp.linInterpol(buffer, width, lcr, lcg, lcb, rcr, rcg, rcb);
	}

	delete tbuffer;
}

void Triang::rastYZ(float* buffer, int height, int width, float* rs, float* gs, float* bs)
{
	float lcr, lcg, lcb, rcr, rcg, rcb;
	float* tbuffer = new float[height * width * 3];
	int j, left, right;

	clearBuffer(tbuffer, height, width);
	
	//for (int i = 0; i < 3; i++)
	//	cout << indices[i] << " rgbs: " << rs[i] << " " << gs[i] << " " << bs[i] << endl;
	for (int i = 0; i < 2; i++)
	{
		Line edge(nys[i], nzs[i], nys[i + 1], nzs[i + 1]);
		edge.linInterpol(tbuffer, width, 1, 1, 1, 1, 1, 1);
		edge.linInterpol(buffer, width, rs[i], gs[i], bs[i], rs[i + 1], gs[i + 1], bs[i + 1]);
	}

	Line edge(nys[2], nzs[2], nys[0], nzs[0]);
	edge.linInterpol(tbuffer, width, 1, 1, 1, 1, 1, 1);
	edge.linInterpol(buffer, width, rs[2], gs[2], bs[2], rs[0], gs[0], bs[0]);

	for (int i = 0; i < height; i++)
	{
		left = -1;
		right = -1;
		j = 0;
		while (j < width)
		{
			if (!checkPix(j, i, tbuffer, width))
				j++;
			else
				break;
		}

		if (j == width) //no pixels on in this row
			continue;
		else
			left = j;

		j++;

		while (j < width) // look for the first pixel not on
		{
			if (checkPix(j, i, tbuffer, width))
			{
				left = j;
				j++;
			}
			else // first pixel not on
				break;
		}

		if (j >= width - 1) //no set of two points on this line
			continue;

		j++;

		while (j < width)
		{
			if (checkPix(j, i, tbuffer, width))
			{
				right = j;
				break;
			}
			else
				j++;
		}

		if (j == width) //no right endpoint found
			continue;

		Line temp(left, i, right, i);
		getColor(left, i, buffer, width, lcr, lcg, lcb);
		getColor(right, i, buffer, width, rcr, rcg, rcb);
		temp.linInterpol(buffer, width, lcr, lcg, lcb, rcr, rcg, rcb);

	}

	delete tbuffer;
}

Triang& Triang::operator=(const Triang &rhs)
{
	if (this == &rhs)
		return *this;

	for (int i = 0; i < 3; i++)
	{
		xvals[i] = rhs.xvals[i];
		yvals[i] = rhs.yvals[i];
		zvals[i] = rhs.zvals[i];
		nxs[i] = rhs.nxs[i];
		nys[i] = rhs.nys[i];
		nzs[i] = rhs.nzs[i];
		indices[i] = rhs.indices[i];
	}

	return *this;
}

int Triang::getIndex(int ind)
{
	return indices[ind];
}
