#ifndef POLY_H
#define POLY_H
#include <vector>
#include <fstream>

class Poly
{
private:
	int numv; // num vertices
	std::vector<float> xvals;
	std::vector<float> yvals;
	std::vector<float> zvals;
	std::vector<std::vector<int> > adj;
	float centx;
	float centy;
	float centz;
	int numAdj;

	void calcCentroid(); //calculate centroid
	void ndc(float& d1, float& d2, float min1, float min2, float mdelta, int N); // normalize the coordinates
public:
	Poly(int numVertex, float* xarray, float* yarray, float* zarray);
	void projectXY(float* buffer, int height, int width, float mdelta, float gminx, float gminy); // project into XY plane
	void projectYZ(float* buffer, int height, int width, float mdelta, float gminy, float gminz); // project into YZ plane
	void projectXZ(float* buffer, int height, int width, float mdelta, float gminx, float gminz); // project into XZ plane
	void translate(float tx, float ty, float tz);
	void rotate(double deg, float p1x, float p1y, float p1z, float p2x, float p2y, float p2z);
	void scale(float alpha, float beta, float gamma);
	int getNumVertex();
	float getx(int index);
	float gety(int index);
	float getz(int index);
	void getMinMax(float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax); //find the min/max of each dimension
	void setAdj(int ind, int newAdj);
	void setNumAdj(int num);
	int getNumAdj();
	void showCoord();
	void printAdj(std::ostream& outfile); //print adj to file
};








#endif
