#include "Helper.h"
#include <string.h>
#include <cstdlib>


void MakePix(int x, int y, float* buffer, int width) // turn on pixel
{
	//float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	//float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	//float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	int location = (y * width * 3) + (x * 3);
	buffer[location] =  .4;
	buffer[location + 1] = 0;
	buffer[location + 2] = .8;
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
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (checkPix(i, j, buffer2, width))
				MakePix(i, j, buffer1, width); // either the pixel is already on in buffer 1 or it'll be turned on	
}

int sbit(int val)
{
	if (val < 0)
		return 1;
	else
		return 0;
}

void turnOffPix(int x, int y, float* buffer, int width)
{
	int location = (y * width * 3) + (x * 3);
	buffer[location] =  0;
	buffer[location + 1] = 0;
	buffer[location + 2] = 0;
}

