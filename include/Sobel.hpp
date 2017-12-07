#ifndef SOBEL
#define SOBEL
#include "Convolutioner.hpp"
class Sobel{
private:

	double sx[9]={
	1,0,-1,
	2,0,-2,
	1,0,-1
	};
	double sy[9]={
	1,2,1,
	0,0,0,
	-1,-2,-1
	};
	uint w;
	uint h;

	Convolutioner* convx;
	Convolutioner* convy;
	Convolutioner* blurConv;

	double* img2norm;
	unsigned char* blurd;
	Image blurImg;
public:
	Sobel(uint iw,uint ih);
	void setBlurOptions(double sigma,uint size);
	void run(Image in,Image* out);

};
#endif