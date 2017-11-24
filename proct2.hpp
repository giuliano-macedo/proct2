#include<stdio.h>
#include "png++/png.hpp"

#define ICFOURIER_SHA 1
#define ICFRACTDIM_SHA 2

#define ICFOURIER_TEX 3
#define ICFRACTDIM_TEX 4

#define IC_MAX_ARG_CODE=4

struct dataSetItem{
	void* data;
	uint label;
}
typedef struct dataSetItem DSItem;
typedef unsigned int uint;
class ImageLoader{
	public:
		ImageLoader(uint* params);
		void addImage(char* path);
		void addParam(uint param);
		DSItem* getDS();

	private:
		DSItem* ds;
		uint* params;
		uint noParams;

}