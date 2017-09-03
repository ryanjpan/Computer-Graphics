#ifndef POLY_H
#define POLY_H
#include <vector>

class Poly
{
private:
	int numv; // num vertices
	std::vector<int> xvals;
	std::vector<int> yvals;
	std::vector<int> maximas;
	int centx;
	int centy;
	bool gone; // if polygon is clipped away

	void setmaximas(); //check if each vertex is a local maxima
	void calcCentroid(); //calculate centroid
public:
	Poly(int numVertex, int* xarray, int* yarray);
	void draw(float* buffer, int width);
	void rasterize(float* buffer, int width, int height);
	int findVertex(int x, int y); //returns index of vertex if found, else -1
	void translate(int tx, int ty);
	void rotate(double deg);
	void scale(double alpha, double beta);
	void clip(int xmin, int xmax, int ymin, int ymax);
	bool diffEdgePoint(int x1, int y1, int x2, int y2);
	bool isGone();
};








#endif
