#ifndef BEZIER_H
#define BEZIER_H
#include <vector>
#include <fstream>

class Bezier
{
private:
	std::vector<float> cpointx;
	std::vector<float> cpointy;
	std::vector<float> ncpointx;
	std::vector<float> ncpointy;
	std::vector<int> curvex;
	std::vector<int> curvey;
	int numcp;

public:
	Bezier(int numbercp, float* cxs, float* cys);
	void getMinMax(float& xmin, float& xmax, float& ymin, float& ymax);
	void ndcAll(float gxmin, float gymin, float mdelta, float N);
	void draw(float* buffer, int width, int resolution);
	void casteljau(float* buffer, int width, float t);
	void printcp();
	void addcp(int index, float x, float y);
	void changecp(int index, float x, float y);
	void deletecp(int index);
	void tofile(std::ofstream& outfile);
};


#endif