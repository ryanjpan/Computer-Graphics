#ifndef HELPER_H
#define HELPER_H

void MakePix(int x, int y, float* buffer, int width);
void clearBuffer(float* buffer, int height, int width);
bool checkPix(int x, int y, float* buffer, int width);
void comBuffer(float* buffer1, float* buffer2, int height, int width);
int sbit(int num); // returns 1 if < 0 and 0 otherwise
void turnOffPix(int x, int y, float* buffer, int width);
void changeColor(float r, float g, float b);


#endif
