#include "Poly.h"
#include "Line.h"
#include "Helper.h"
#include <math.h>
#include <iostream>
#include <algorithm>

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
		vertexr.insert(vertexr.end(), 0);
		vertexg.insert(vertexg.end(), 0);
		vertexb.insert(vertexb.end(), 0);
		normalx.insert(normalx.end(), 0);
		normaly.insert(normaly.end(), 0);
		normalz.insert(normalz.end(), 0);
	}

}

void Poly::ndcTriangs(float mdelta, float gminx, float gminy, float gminz, int N)
{
	for (int i = 0; i < numTriang; i++)
	{
		triangles[i].ndcAll(mdelta, gminx, gminy, gminz, N);
	}
}

void Poly::rasterizeXY(float* buffer, int height, int width)
{

	Triang temp;
	float rs[3], gs[3], bs[3];
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (triangles[j].getsz() > triangles[j+1].getsz())
			{
				temp = triangles[j+1];
				triangles[j+1] = triangles[j];
				triangles[j] = temp;
			}
		}
	}

	for (int i = 0; i < numTriang; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			rs[j] = vertexr[triangles[i].getIndex(j)];
			gs[j] = vertexg[triangles[i].getIndex(j)];
			bs[j] = vertexb[triangles[i].getIndex(j)];
		}
		triangles[i].rastXY(buffer, height, width, rs, gs, bs);
	}

}

void Poly::rasterizeYZ(float* buffer, int height, int width)
{

	Triang temp;
	float rs[3], gs[3], bs[3];

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (triangles[j].getsx() > triangles[j+1].getsx())
			{
				temp = triangles[j+1];
				triangles[j+1] = triangles[j];
				triangles[j] = temp;
			}
		}
	}

	for (int i = 0; i < numTriang; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			rs[j] = vertexr[triangles[i].getIndex(j)];
			gs[j] = vertexg[triangles[i].getIndex(j)];
			bs[j] = vertexb[triangles[i].getIndex(j)];
		}
		triangles[i].rastYZ(buffer, height, width, rs, gs, bs);
	}

}

void Poly::rasterizeXZ(float* buffer, int height, int width)
{
	
	Triang temp;
	float rs[3], gs[3], bs[3];

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (triangles[j].getsy() > triangles[j+1].getsy())
			{
				temp = triangles[j+1];
				triangles[j+1] = triangles[j];
				triangles[j] = temp;
			}
		}
	}

	for (int i = 0; i < numTriang; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			rs[j] = vertexr[triangles[i].getIndex(j)];
			gs[j] = vertexg[triangles[i].getIndex(j)];
			bs[j] = vertexb[triangles[i].getIndex(j)];
		}
		triangles[i].rastXZ(buffer, height, width, rs, gs, bs);
	}

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

void Poly::getMinMaxC(float& rmin, float& rmax, float& gmin, float& gmax, float& bmin, float& bmax) // returns the smallest and largest number in each dimension
{
	rmin = rmax = vertexr[0]; // initialize both min and max of each dimension to the first vertex
	gmin = gmax = vertexg[0];
	bmin = bmax = vertexb[0];
	for (int i = 1; i < numv; i++)
	{
		if (vertexr[i] < rmin)
			rmin = vertexr[i];
		else if (vertexr[i] > rmax)
			rmax = vertexr[i];

		if (vertexg[i] < gmin)
			gmin = vertexg[i];
		else if (vertexg[i] > gmax)
			gmax = vertexg[i];

		if (vertexb[i] < bmin)
			bmin = vertexb[i];
		else if (vertexb[i] > bmax)
			bmax = vertexb[i];
	}
}

void Poly::ndcC(float rdelta, float gdelta, float bdelta, float gminr, float gming, float gminb)
{
	for(int i = 0; i < numv; i++)
	{
		vertexr[i] = (vertexr[i] - gminr) / rdelta;
		vertexg[i] = (vertexg[i] - gming) / gdelta;
		vertexb[i] = (vertexb[i] - gminb) / bdelta;
	}
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

void Poly::setNumTriang(int val)
{
	numTriang = val;
}

float Poly::getsx() // returns max so sort is correct
{
	float val = xvals[0];
	for (int i = 1; i < numv; i++)
	{
		if (xvals[i] > val)
			val = xvals[i];
	}

	return val;
}

float Poly::getsy()
{
	float val = yvals[0];
	for (int i = 1; i < numv; i++)
	{
		if (yvals[i] > val)
			val = yvals[i];
	}

	return val;
}

float Poly::getsz()
{
	float val = zvals[0];
	for (int i = 1; i > numv; i++)
	{
		if (zvals[i] < val)
			val = zvals[i];
	}

	return val;
}

void Poly::makeTriang(int i1, int i2, int i3)
{
	float txs[3], tys[3], tzs[3];

	txs[0] = xvals[i1];
	txs[1] = xvals[i2];
	txs[2] = xvals[i3];

	tys[0] = yvals[i1];
	tys[1] = yvals[i2];
	tys[2] = yvals[i3];

	tzs[0] = zvals[i1];
	tzs[1] = zvals[i2];
	tzs[2] = zvals[i3];

	Triang temp(txs, tys, tzs, i1, i2, i3);
	triangles.push_back(temp);
}

void Poly::setNormal(int index, float nx, float ny, float nz)
{
	float magn = sqrt(nx*nx + ny*ny + nz*nz);
	normalx[index] = nx / magn;
	normaly[index] = ny / magn;
	normalz[index] = nz / magn;
}

void Poly::phong(float fx, float fy, float fz, float kar, float kag, float kab, float ksr, float ksg, float ksb, float kdr, float kdg, float kdb, float lx, float ly, float lz, float Iar, float Iag, float Iab, float ILr, float ILg, float ILb, int PN)
{
	float llx, lly, llz, rx, ry, rz, magl, ndotll, vx, vy, vz, magv, calpha, cbeta, magr;

	for (int i = 0; i < numv; i++)
	{
		llx = (lx - xvals[i]); // light vector
		lly = (ly - yvals[i]);
		llz = (lz - zvals[i]);
		magl = sqrt(llx*llx + lly*lly + llz*llz);
		llx = llx / magl;
		lly = lly / magl;
		llz = llz / magl;

		//cout << "light vec: " << llx << " " << lly << " " << llz << endl;

		ndotll = llx * normalx[i] + lly * normaly[i] + llz * normalz[i];

		rx = -llx + 2 * ndotll * normalx[i]; // reflection vector
		ry = -lly + 2 * ndotll * normaly[i];
		rz = -llz + 2 * ndotll * normalz[i];
		magr = sqrt(rx*rx + ry*ry + rz*rz);
		rx = rx/magr;
		ry = ry/magr;
		rz = rz/magr;
		//cout << "reflect vec: " << rx << " " << ry << " " << rz << endl;

		vx = fx - xvals[i]; // viewing vector
		vy = fy - yvals[i];
		vz = fz - zvals[i];
		magv = sqrt(vx*vx + vy*vy + vz*vz);
		vx = vx / magv;
		vy = vy / magv;
		vz = vz / magv;

		//cout << "viewing vec: " << vx << " " << vy << " " << vz << endl;

		calpha = normalx[i] * llx + normaly[i] * lly + normalz[i] * llz;
		cbeta = rx * vx + ry * vy + rz * vz;

		//cout << "calpha: " << calpha << endl;
		//cout << "cbeta: " << cbeta << endl;

		vertexr[i] = kar * Iar + ILr / (magl + magv) * (kdr * calpha + ksr * pow(cbeta, PN));
		vertexg[i] = kag * Iag + ILg / (magl + magv) * (kdg * calpha + ksg * pow(cbeta, PN));
		vertexb[i] = kab * Iab + ILb / (magl + magv) * (kdb * calpha + ksb * pow(cbeta, PN));

	}

}
