#include "Bezier.h"
#include "Helper.h"
#include "Line.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;

Bezier::Bezier(int numbercp, float* cxs, float* cys)
{
	numcp = numbercp;
	for (int i = 0; i < numbercp; i++)
	{
		cpointx.push_back(cxs[i]);
		cpointy.push_back(cys[i]);
	}

}

void Bezier::getMinMax(float& xmin, float& xmax, float& ymin, float& ymax) // returns the smallest and largest number in each dimension
{
	xmin = xmax = cpointx[0]; // initialize both min and max of each dimension to the first vertex
	ymin = ymax = cpointy[0];
	for (int i = 1; i < numcp; i++)
	{
		//cout << cpointx[i] << " " << cpointy[i] << endl;
		if (cpointx[i] < xmin)
			xmin = cpointx[i];
		else if (cpointx[i] > xmax)
			xmax = cpointx[i];

		if (cpointy[i] < ymin)
			ymin = cpointy[i];
		else if (cpointy[i] > ymax)
			ymax = cpointy[i];

	}
}

void Bezier::ndcAll(float gxmin, float gymin, float mdelta, float N)
{
	float cx, cy;
	ncpointx.clear();
	ncpointy.clear();

	for (int i = 0; i < numcp; i++)
	{
		cx = ((cpointx[i] - gxmin) / mdelta) * (N - 1); //normalize x
		ncpointx.insert(ncpointx.end(), cx);

		cy = ((cpointy[i] - gymin) / mdelta) * (N - 1); //normalize y
		ncpointy.insert(ncpointy.end(), cy);
	}
}

void Bezier::draw(float* buffer, int width, int resolution)
{
	curvex.clear();
	curvey.clear();
	if (resolution == 0 || resolution == 1)
	{
		cout << "Bad Bezier resolution" << endl;
		return;
	}
	float t;
	for (int i = 0; i <= resolution; i++)
	{
		t = float(i) / resolution;
		casteljau(buffer, width, t);
	}

	changeColor(1, 0, 0);
	for (int i = 0; i < ncpointx.size() - 1; i++)
	{
		Line temp(ncpointx[i], ncpointy[i], ncpointx[i + 1], ncpointy[i + 1]);
		//cout << ncpointx[i] << " " << ncpointy[i] << " " << ncpointx[i + 1] << " " << ncpointy[i + 1] << endl;
		temp.ddaDraw(buffer, width);
	}

	changeColor(.4, 0, .8);
	for (int i = 0; i < curvex.size() - 1; i++)
	{
		Line temp(curvex[i], curvey[i], curvex[i + 1], curvey[i + 1]);
		//cout << curvex[i] << " " << curvey[i] << " " << curvex[i + 1] << " " << curvey[i + 1] << endl;
		temp.ddaDraw(buffer, width);
	}

}

void Bezier::casteljau(float* buffer, int width, float t)
{
	vector<vector<float> > xgen;
	vector<vector<float> > ygen;
	vector<float> temp;
	float lbx, lby;
	int x, y;

	for (int i = 0; i < numcp; i++) // initialize vector of vectors
	{
		xgen.insert(xgen.end(), temp);
		ygen.insert(ygen.end(), temp);
	}

	for (int i = 0; i < numcp; i++)
	{
		xgen[0].insert(xgen[0].end(), ncpointx[i]);
		ygen[0].insert(ygen[0].end(), ncpointy[i]);
	}

	for (int j = 1; j < numcp; j++)
	{
		for (int i = 0; i < numcp - j; i++)
		{
			lbx = (1 - t) * xgen[j - 1][i] + t * xgen[j - 1][i + 1];
			lby = (1 - t) * ygen[j - 1][i] + t * ygen[j - 1][i + 1];
			xgen[j].insert(xgen[j].end(), lbx);
			ygen[j].insert(ygen[j].end(), lby);
		}
	}

	x = round(xgen[numcp - 1][0]);
	y = round(ygen[numcp - 1][0]);
	curvex.insert(curvex.end(), x);
	curvey.insert(curvey.end(), y);
}

void Bezier::printcp()
{
	cout << "Control Points" << endl;
	cout << "-----------------------" << endl;
	for (int i = 0; i < numcp; i++)
	{
		cout << "Control Point " << i << ": " << cpointx[i] << " " << cpointy[i] << endl;
	}
}

void Bezier::addcp(int index, float x, float y)
{
	cpointx.insert(cpointx.begin() + index, x);
	cpointy.insert(cpointy.begin() + index, y);
	numcp++;
}

void Bezier::changecp(int index, float x, float y)
{
	cpointx[index] = x;
	cpointy[index] = y;

}

void Bezier::deletecp(int index)
{
	cpointx.erase(cpointx.begin() + index);
	cpointy.erase(cpointy.begin() + index);

	numcp--;
}

void Bezier::tofile(ofstream& outfile)
{
	outfile << "bz" << endl;
	outfile << numcp << endl;
	for (int i = 0; i < numcp; i++)
	{
		outfile << cpointx[i] << " " << cpointy[i] << endl;
	}
	outfile << endl;
}