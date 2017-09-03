#include "Helper.h"
#include <string.h>
#include <cstdlib>
#include <iostream>

using namespace std;

float colorr = .4;
float colorg = 0;
float colorb = .8;

void MakePix(int x, int y, float* buffer, int width) // turn on pixel
{
	/*
	if (x < 0 || x > width - 1)
	{
		cout << "MakePix out of bounds" << endl;
	}

	if (y < 0 || y > width - 1)
	{
		cout << "MakePix out of bounds" << endl;
	}*/
	//float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	//float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	//float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	int location = (y * width * 3) + (x * 3);
	buffer[location] =  colorr;
	buffer[location + 1] = colorg;
	buffer[location + 2] = colorb;
}

void ColorPix(int x, int y, float* buffer, int width, float r, float g, float b)
{
	int location = (y * width * 3) + (x * 3);
	buffer[location] = r;
	buffer[location + 1] = g;
	buffer[location + 2] = b;
}

void getColor(int x, int y, float* buffer, int width, float& valr, float& valg, float& valb)
{
	int location = (y * width * 3) + (x * 3);
	valr = buffer[location];
	valg = buffer[location + 1];
	valb = buffer[location + 2];
}

void clearBuffer(float* buffer, int height, int width) // set all values to 0
{
	memset(buffer, 0, sizeof(float) * height * width * 3);
}

bool checkPix(int x, int y, float* buffer, int width) // if pixel is on return true
{
	int location = (y * width * 3) + (x * 3);

	if (buffer[location] != 0 || buffer[location + 1] != 0 || buffer[location + 2] != 0) // assumes that only 0,0,0 is off for a pixel
		return true;

	else
		return false;
}

void comBuffer(float* buffer1, float* buffer2, int height, int width) // combines buffers into buffer 1
{
	float r, g, b;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			if (checkPix(i, j, buffer2, width))
			{
				getColor(i, j, buffer2, width, r, g, b);
				ColorPix(i, j, buffer1, width, r, g, b);
			}
		}
}

int sbit(int val)
{
	if (val < 0)
		return 1;
	else
		return 0;
}

void OnPix(int x, int y, float* buffer, int width)
{
	int location = (y * width * 3) + (x * 3);
	buffer[location] = 1;
	buffer[location + 1] = 1;
	buffer[location + 2] = 1;
}

void OffPix(int x, int y, float* buffer, int width)
{
	int location = (y * width * 3) + (x * 3);
	buffer[location] =  0;
	buffer[location + 1] = 0;
	buffer[location + 2] = 0;
}

void changeColor(float r, float g, float b)
{
	colorr = r;
	colorg = g;
	colorb = b;
}

void Halftone(float* buffer, int height, int width)
{
	if (height % 3 != 0 || width % 3 != 0)
	{
		cout << "Bad Dimensions" << endl;
		return;
	}

	int intensity;
	float sumr=0, sumg=0, sumb=0, cr, cg, cb;

	for (int i = 1; i < height; i = i + 3)
	{
		for (int j = 1; j < width; j = j + 3)
		{
			sumr = 0;
			sumg = 0;
			sumb = 0;
			getColor(i-1, j-1, buffer, width, cr, cg, cb);
			sumr += cr;
			sumg += cg;
			sumb += cb;
			getColor(i - 1, j, buffer, width, cr, cg, cb);
			sumr += cr;
			sumg += cg;
			sumb += cb;
			getColor(i - 1, j + 1, buffer, width, cr, cg, cb);
			sumr += cr;
			sumg += cg;
			sumb += cb;
			getColor(i, j - 1, buffer, width, cr, cg, cb);
			sumr += cr;
			sumg += cg;
			sumb += cb;
			getColor(i, j, buffer, width, cr, cg, cb);
			sumr += cr;
			sumg += cg;
			sumb += cb;
			getColor(i, j + 1, buffer, width, cr, cg, cb);
			sumr += cr;
			sumg += cg;
			sumb += cb;
			getColor(i + 1, j - 1, buffer, width, cr, cg, cb);
			sumr += cr;
			sumg += cg;
			sumb += cb;
			getColor(i + 1, j, buffer, width, cr, cg, cb);
			sumr += cr;
			sumg += cg;
			sumb += cb;
			getColor(i + 1, j + 1, buffer, width, cr, cg, cb);
			sumr += cr;
			sumg += cg;
			sumb += cb;
			intensity = int((sumr / 10 + sumg / 10 + sumb / 10) * 10 / 3);
			switch (intensity)
			{
			case 0: ht0(i, j, buffer, width);
				break;
			case 1: ht1(i, j, buffer, width);
				break;
			case 2: ht2(i, j, buffer, width);
				break;
			case 3: ht3(i, j, buffer, width);
				break;
			case 4: ht4(i, j, buffer, width);
				break;
			case 5: ht5(i, j, buffer, width);
				break;
			case 6: ht6(i, j, buffer, width);
				break;
			case 7: ht7(i, j, buffer, width);
				break;
			case 8: ht8(i, j, buffer, width);
				break;
			case 9: ht9(i, j, buffer, width);
				break;
			default:
				cout << "Bad Half Tone Intensity" << endl;
			}
		}
	}
}

void ht0(int x, int y, float* buffer, int width)
{
	OffPix(x - 1, y - 1, buffer, width);
	OffPix(x - 1, y, buffer, width);
	OffPix(x - 1, y + 1, buffer, width);
	OffPix(x, y - 1, buffer, width);
	OffPix(x, y, buffer, width);
	OffPix(x, y + 1, buffer, width);
	OffPix(x + 1, y - 1, buffer, width);
	OffPix(x + 1, y, buffer, width);
	OffPix(x + 1, y + 1, buffer, width);
}

void ht1(int x, int y, float* buffer, int width)
{
	OffPix(x - 1, y - 1, buffer, width);
	OffPix(x - 1, y, buffer, width);
	OffPix(x - 1, y + 1, buffer, width);
	OffPix(x, y - 1, buffer, width);
	OnPix(x, y, buffer, width);
	OffPix(x, y + 1, buffer, width);
	OffPix(x + 1, y - 1, buffer, width);
	OffPix(x + 1, y, buffer, width);
	OffPix(x + 1, y + 1, buffer, width);
}

void ht2(int x, int y, float* buffer, int width)
{
	OffPix(x - 1, y - 1, buffer, width);
	OffPix(x - 1, y, buffer, width);
	OnPix(x - 1, y + 1, buffer, width);
	OffPix(x, y - 1, buffer, width);
	OffPix(x, y, buffer, width);
	OffPix(x, y + 1, buffer, width);
	OnPix(x + 1, y - 1, buffer, width);
	OffPix(x + 1, y, buffer, width);
	OffPix(x + 1, y + 1, buffer, width);
}

void ht3(int x, int y, float* buffer, int width)
{
	OffPix(x - 1, y - 1, buffer, width);
	OffPix(x - 1, y, buffer, width);
	OnPix(x - 1, y + 1, buffer, width);
	OffPix(x, y - 1, buffer, width);
	OnPix(x, y, buffer, width);
	OffPix(x, y + 1, buffer, width);
	OnPix(x + 1, y - 1, buffer, width);
	OffPix(x + 1, y, buffer, width);
	OffPix(x + 1, y + 1, buffer, width);
}

void ht4(int x, int y, float* buffer, int width)
{
	OnPix(x - 1, y - 1, buffer, width);
	OffPix(x - 1, y, buffer, width);
	OnPix(x - 1, y + 1, buffer, width);
	OffPix(x, y - 1, buffer, width);
	OffPix(x, y, buffer, width);
	OffPix(x, y + 1, buffer, width);
	OnPix(x + 1, y - 1, buffer, width);
	OffPix(x + 1, y, buffer, width);
	OnPix(x + 1, y + 1, buffer, width);
}

void ht5(int x, int y, float* buffer, int width)
{
	OnPix(x - 1, y - 1, buffer, width);
	OffPix(x - 1, y, buffer, width);
	OnPix(x - 1, y + 1, buffer, width);
	OffPix(x, y - 1, buffer, width);
	OnPix(x, y, buffer, width);
	OffPix(x, y + 1, buffer, width);
	OnPix(x + 1, y - 1, buffer, width);
	OffPix(x + 1, y, buffer, width);
	OnPix(x + 1, y + 1, buffer, width);
}

void ht6(int x, int y, float* buffer, int width)
{
	OnPix(x - 1, y - 1, buffer, width);
	OffPix(x - 1, y, buffer, width);
	OnPix(x - 1, y + 1, buffer, width);
	OnPix(x, y - 1, buffer, width);
	OffPix(x, y, buffer, width);
	OnPix(x, y + 1, buffer, width);
	OnPix(x + 1, y - 1, buffer, width);
	OffPix(x + 1, y, buffer, width);
	OnPix(x + 1, y + 1, buffer, width);
}

void ht7(int x, int y, float* buffer, int width)
{
	OnPix(x - 1, y - 1, buffer, width);
	OffPix(x - 1, y, buffer, width);
	OnPix(x - 1, y + 1, buffer, width);
	OnPix(x, y - 1, buffer, width);
	OnPix(x, y, buffer, width);
	OnPix(x, y + 1, buffer, width);
	OnPix(x + 1, y - 1, buffer, width);
	OffPix(x + 1, y, buffer, width);
	OnPix(x + 1, y + 1, buffer, width);
}

void ht8(int x, int y, float* buffer, int width)
{
	OnPix(x - 1, y - 1, buffer, width);
	OnPix(x - 1, y, buffer, width);
	OnPix(x - 1, y + 1, buffer, width);
	OnPix(x, y - 1, buffer, width);
	OffPix(x, y, buffer, width);
	OnPix(x, y + 1, buffer, width);
	OnPix(x + 1, y - 1, buffer, width);
	OnPix(x + 1, y, buffer, width);
	OnPix(x + 1, y + 1, buffer, width);
}

void ht9(int x, int y, float* buffer, int width)
{
	OnPix(x - 1, y - 1, buffer, width);
	OnPix(x - 1, y, buffer, width);
	OnPix(x - 1, y + 1, buffer, width);
	OnPix(x, y - 1, buffer, width);
	OnPix(x, y, buffer, width);
	OnPix(x, y + 1, buffer, width);
	OnPix(x + 1, y - 1, buffer, width);
	OnPix(x + 1, y, buffer, width);
	OnPix(x + 1, y + 1, buffer, width);
}