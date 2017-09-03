#include "Spline.h"
#include "Helper.h"
#include "Line.h"
#include <iostream>
#include <fstream>

using namespace std;

Spline::Spline(int numbercp, int kval, float* cxs, float* cys, float* tknots)
{
	numcp = numbercp;
	k = kval;
	for (int i = 0; i < numbercp; i++)
	{
		cpointx.push_back(cxs[i]);
		cpointy.push_back(cys[i]);
	}


	for (int i = 0; i < numcp + kval; i++)
	{
		knots.push_back(tknots[i]);
	}

}

Spline::Spline(int numbercp, int kval, float* cxs, float* cys)
{
	numcp = numbercp;
	k = kval;
	for (int i = 0; i < numbercp; i++)
	{
		cpointx.push_back(cxs[i]);
		cpointy.push_back(cys[i]);
	}


	for (int i = 0; i < numcp + kval; i++)
	{
		knots.push_back(i);
	}
}

int Spline::ksearch(float ubar)
{
	for (int i = 0; i < knots.size() - 1; i++)
		if (ubar >= knots[i] && ubar < knots[i + 1])
			return i;

	return -1; // Should never reach here, indicates error
}

void Spline::getMinMax(float& xmin, float& xmax, float& ymin, float& ymax) // returns the smallest and largest number in each dimension
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

void Spline::ndcAll(float gxmin, float gymin, float mdelta, float N)
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

void Spline::draw(float* buffer, int width, int resolution)
{
	curvex.clear();
	curvey.clear();
	float ulower = knots[k - 1];
	float uupper = knots[numcp];
	float udelta = uupper - ulower;
	float ubar;

	for (int i = 0; i <= resolution; i++)
	{
		ubar = float(i) / resolution * udelta + ulower;
		deBoor(ubar);
	}
	
	changeColor(1, 0, 0);
	for (int i = 0; i < ncpointx.size() - 1; i++)
	{
		Line temp(ncpointx[i], ncpointy[i], ncpointx[i + 1], ncpointy[i + 1]);
		temp.ddaDraw(buffer, width);
	}

	changeColor(.4, 0, .8);
	for (int i = 0; i < curvex.size() - 1; i++)
	{
		Line temp(curvex[i], curvey[i], curvex[i + 1], curvey[i + 1]);
		temp.ddaDraw(buffer, width);
	}
}

void Spline::deBoor(float ubar)
{
	vector<vector<float> > colx; // columns
	vector<vector<float> > coly;
	vector<float> temp;
	int I = ksearch(ubar);
	float delta, xval, yval;

	for (int i = 0; i < numcp + k; i++)
	{
		temp.push_back(0);
	}

	for (int i = 0; i < k; i++)
	{
		colx.push_back(temp);
		coly.push_back(temp);
	}

	for (int i = 0; i < numcp; i++)
	{
		colx[0][i] = ncpointx[i];
		coly[0][i] = ncpointy[i];
	}

	for (int j = 1; j < k; j++)
	{
		for (int i = I - (k - 1); i <= I - j; i++)
		{
			delta = knots[i + k] - knots[i + j];
			xval = (knots[i + k] - ubar) / delta * colx[j - 1][i] + (ubar - knots[i + j]) / delta * colx[j - 1][i + 1];
			yval = (knots[i + k] - ubar) / delta * coly[j - 1][i] + (ubar - knots[i + j]) / delta * coly[j - 1][i + 1];
			colx[j][i] = xval;
			coly[j][i] = yval;
		}
	}

	//cout << colx[k - 1][I - (k - 1)] << coly[k - 1][I - (k - 1)] << endl;

	curvex.push_back(colx[k - 1][I - (k - 1)]);
	curvey.push_back(coly[k - 1][I - (k - 1)]);
}

void Spline::printcpknots()
{
	cout << "Control Points" << endl;
	cout << "-----------------------" << endl;
	for (int i = 0; i < numcp; i++)
	{
		cout << "Control Point " << i << ": " << cpointx[i] << " " << cpointy[i] << endl;
	}

	cout << "knots: ";
	for (int i = 0; i < knots.size(); i++)
	{
		cout << knots[i] << " ";
	}
	cout << endl;
}

int Spline::getk()
{
	return k;
}

int Spline::getnumcp()
{
	return numcp;
}

void Spline::modifyknots(float* tknots)
{
	knots.clear();
	for (int i = 0; i < numcp + k; i++)
	{
		knots.push_back(tknots[i]);
	}
}

void Spline::setk(int kval)
{
	k = kval;
}

void Spline::modifycp(int index, float nx, float ny)
{
	cpointx[index] = nx;
	cpointy[index] = ny;
}

void Spline::addcp(float nx, float ny)
{
	cpointx.push_back(nx);
	cpointy.push_back(ny);
	numcp++;
}

void Spline::deletecp(int index)
{
	cpointx.erase(cpointx.begin() + index);
	cpointy.erase(cpointy.begin() + index);

	numcp--;
}

void Spline::tofile(ofstream& outfile)
{
	outfile << "bs" << endl;
	outfile << numcp << endl;
	outfile << k << endl;
	outfile << 'T' << endl;
	for (int i = 0; i < numcp; i++)
	{
		outfile << cpointx[i] << " " << cpointy[i] << endl;
	}
	for (int i = 0; i < numcp + k; i++)
	{
		outfile << knots[i] << " ";
	}
	outfile << endl << endl;
}