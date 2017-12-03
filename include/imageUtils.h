#include "lodepng.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
typedef unsigned int uint;
typedef struct Image{
	unsigned char* data;
	uint w;
	uint h;
} Image;
Image loadImage(const char* filename);
Image newImageFrom(Image* i);
Image newImage(uint h,uint w);
int saveImage(Image *i,const char* filename);
void scaleImageLIN(Image in,Image* out,float dx,float dy);