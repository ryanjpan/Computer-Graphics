#ifndef HELPER_H
#define HELPER_H

void MakePix(int x, int y, float* buffer, int width);
void ColorPix(int x, int y, float* buffer, int width, float r, float g, float b);
void getColor(int x, int y, float* buffer, int width, float& valr, float& valg, float& valb);
void clearBuffer(float* buffer, int height, int width);
bool checkPix(int x, int y, float* buffer, int width);
void comBuffer(float* buffer1, float* buffer2, int height, int width);
int sbit(int num); // returns 1 if < 0 and 0 otherwise
void OnPix(int x, int y, float* buffer, int width);
void OffPix(int x, int y, float* buffer, int width);
void changeColor(float r, float g, float b);
void Halftone(float* buffer, int height, int width);
void ht0(int x, int y, float* buffer, int width);
void ht1(int x, int y, float* buffer, int width);
void ht2(int x, int y, float* buffer, int width);
void ht3(int x, int y, float* buffer, int width);
void ht4(int x, int y, float* buffer, int width);
void ht5(int x, int y, float* buffer, int width);
void ht6(int x, int y, float* buffer, int width);
void ht7(int x, int y, float* buffer, int width);
void ht8(int x, int y, float* buffer, int width);
void ht9(int x, int y, float* buffer, int width);

#endif
