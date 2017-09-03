#ifndef POLY_H
#define POLY_H
#include <vector>
#include <fstream>
#include "Triang.h"

class Poly
{
private:
	int numv; // num vertices
	std::vector<float> xvals;
	std::vector<float> yvals;
	std::vector<float> zvals;
	std::vector<std::vector<int> > adj;
	std::vector<float> normalx;
	std::vector<float> normaly;
	std::vector<float> normalz;
	std::vector<float> vertexr;
	std::vector<float> vertexg;
	std::vector<float> vertexb;
	
	int numAdj;
	int numTriang;
	std::vector<Triang> triangles;

public:
	Poly(int numVertex, float* xarray, float* yarray, float* zarray);
	void ndcTriangs(float mdelta, float gminx, float gminy, float gminz, int N);
	void rasterizeXY(float* buffer, int height, int width);
	void rasterizeYZ(float* buffer, int height, int width);
	void rasterizeXZ(float* buffer, int height, int width);
	int getNumVertex();
	float getx(int index);
	float gety(int index);
	float getz(int index);
	float getsx();
	float getsy();
	float getsz();
	void getMinMax(float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax); //find the min/max of each dimension
	void getMinMaxC(float& rmin, float& rmax, float& gmin, float& gmax, float& bmin, float& bmax);
	void ndcC(float rdelta, float gdelta, float bdelta, float gminr, float gming, float gminb);
	void setAdj(int ind, int newAdj);
	void setNumAdj(int num);
	int getNumAdj();
	void setNumTriang(int val);
	void makeTriang(int i1, int i2, int i3);
	void setNormal(int index, float nx, float ny, float nz);
	void phong(float fx, float fy, float fz, float kar, float kag, float kab, float ksr, float ksg, float ksb, float kdr, float kdg, float kdb, float lx, float ly, float lz, float Iar, float Iag, float Iab, float ILr, float ILg, float ILb, int PN);
};








#endif
