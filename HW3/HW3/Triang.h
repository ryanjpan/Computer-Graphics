#ifndef TRIANG_H
#define TRIANG_H
#include <fstream>

class Triang
{
private:
	float xvals[3];
	float yvals[3];
	float zvals[3];

	int nxs[3];
	int nys[3]; //normalized coordinates for Buffers
	int nzs[3];

	int indices[3];
	//void calcCentroid();
	int ndc(float val, float min, float mdelta, int N); // normalize the coordinates
public:
	Triang(float* xarray, float* yarray, float* zarray, int i1, int i2, int i3);
	Triang();
	void ndcAll(float mdelta, float gminx, float gminy, float gminz, int N); // project into XY plane
	void rastXY(float* buffer, int height, int width, float* rs, float* gs, float* bs);
	void rastYZ(float* buffer, int height, int width, float* rs, float* gs, float* bs);
	void rastXZ(float* buffer, int height, int width, float* rs, float* gs, float* bs); // project into XZ plane
	//void projectXZ(float* buffer, int height, int width, float mdelta, float gminx, float gminz); // project into XZ plane
	float getsx();
	float getsy();
	float getsz();
	int getIndex(int ind);
	Triang& operator= (const Triang &rhs);

};








#endif
