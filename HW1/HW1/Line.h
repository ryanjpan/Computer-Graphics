#ifndef LINE_H
#define LINE_H

class Line
{
private:
	int leftx;
	int lefty;
	int rightx;
	int righty;
	bool gone; //if clipped away

	Line();

public:
	Line(int lx, int ly, int rx, int ry);
	void ddaDraw(float* buffer, int width);
	void bresDraw(float* buffer, int width);
	void plot(float* buffer, int width); // plots only the endpoints
	bool horiz(); // check if line is horizontal
	void clip(int xmin, int xmax, int ymin, int ymax); // x, y min max are clipping window boundaries
	bool isGone(); // checks if line has been clipped away
	bool diffEdge(int x1, int y1, int x2, int y2); // checks if one point is from this line but the other is not
	bool isOnLine(int x, int y); // checks if a point is on this line
	void horzsect(int yval, int& xint, int& yint); // gives the intersection with horizontal line into xint and yint
	void vertsect(int xval, int& xint, int& yint); // gives the intersection with vertical line into xint and yint
	
};















#endif
