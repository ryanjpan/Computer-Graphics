#ifndef SPLINE_H
#define SPLINE_H
#include <vector>
#include <fstream>

class Spline
{
private:
	std::vector<int> cpointx;
	std::vector<int> cpointy;
	std::vector<float> ncpointx;
	std::vector<float> ncpointy;
	std::vector<int> curvex;
	std::vector<int> curvey;
	std::vector<float> knots;
	int numcp;
	int k;

	int ksearch(float ubar); //searches and returns the preceding index
public:
	Spline(int numbercp, int kval, float* cxs, float* cys, float* tknots);
	Spline(int numbercp, int kval, float* cxs, float* cys);
	void getMinMax(float& xmin, float& xmax, float& ymin, float& ymax);
	void ndcAll(float gxmin, float gymin, float mdelta, float N);
	void draw(float* buffer, int width, int resolution);
	void deBoor(float ubar);
	void printcpknots();
	int getk();
	int getnumcp();
	void modifyknots(float* tknots);
	void setk(int kval);
	void modifycp(int index, float nx, float ny);
	void addcp(float nx, float ny);
	void deletecp(int index);
	void tofile(std::ofstream& outfile);
};


#endif