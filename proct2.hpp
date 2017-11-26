#include<stdio.h>
#include "lode/lode.h"

enum IcParamCode{
	ICFOURIER_SHA,
	ICFRACTDIM_SHA,

	ICFOURIER_TEX,
	ICFRACTDIM_TEX,
	ICLBP_TEX,
	
	iC_NO_ARGTYPES
};

struct dataSetItem{
	double* data;
	uint label;
};
struct Image{
	unsigned char* data;
	uint w;
	uint h;
};
typedef struct dataSetItem DSItem;
typedef struct Image Image;
typedef unsigned int uint;
class ImageLoader{
	public:
		ImageLoader();
		void addImage(const char* filename);
		void addParam(IcParamCode param);
		DSItem* getDS();

	private:
		void (* preProcFs[iC_NO_ARGTYPES])(Image* img);
		uint noParams;
		static Image getImage(const char* filename);
		DSItem* ds;

}