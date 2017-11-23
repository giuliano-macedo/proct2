#include<stdio.h>
#include "png++/png.hpp"
struct dataSetItem{
	void* data;
	uint label;
}
typedef struct dataSetItem DSItem;
typedef unsigned int uint;
class ImageCategorizer{
	public:
		ImageCategorizer(uint* params);
		void addImage(char* path);
		void addParam(uint param);
		DSItem* getDS();

		const uint FOURIER_TEX=1;
		const uint FRACTDIM_SHAPE=2;

		const uint FOURIER_TEX=3;
		const uint FRACTDIM_TEX=4;
	private:
		DSItem* ds;

}