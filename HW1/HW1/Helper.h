#ifndef HELPER_H
#define HELPER_H

void MakePix(int x, int y, float* buffer, int width);
void clearBuffer(float* buffer, int height, int width);
bool checkPix(int x, int y, float* buffer, int width);
void comBuffer(float* buffer1, float* buffer2, int height, int width);
int sbit(int num);
void turnOffPix(int x, int y, float* buffer, int width);


#endif
