#ifndef CONVOLUTIONER

#define CONVOLUTIONER
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "imageUtils.h"
#define PI 3.141592653589793
typedef struct Kernel{
	double* data;
	uint w;
	uint h;
} Kernel;
class Convolutioner{
private:
	double* ker;
	uint kw;
	uint kh;
	uint hkw;
	uint hkh;
	uint ks;

	unsigned char* img;
	uint imgw;
	uint imgh;
	uint imgs;

public:
	Convolutioner(Kernel k);
	void setImage(Image in);
	void setKernel(Kernel k);
	double convoluteAt(uint i,uint j);
};
void printKer(Kernel ker);
void normalizeKernel(Kernel *k);
Kernel genGaussKernel(uint size,double s);
#endif