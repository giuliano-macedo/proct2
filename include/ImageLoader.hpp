#ifndef IMAGELOADER

#define IMAGELOADER
#include <stdio.h>
#include "lodepng.h"
#include "fft.h"
#include "imageUtils.h"
#include "Sobel.hpp"
#include <math.h>
#include <vector>
#include <string>
enum IcParamCode{
	ILFRACTDIM_SHA=1,
	ILHUMOMENTS_SHA,

	ILFOURIER_TEX,
	ILNETACTIVITY_TEX,

	IL_NO_ARGTYPES
};

struct dataSetItem{
	std::vector<double> data;
	uint label;
};

typedef struct dataSetItem DSItem;
typedef unsigned int uint;

class ImageLoader {
	public:
		ImageLoader();
		void processImage(const char* filename,uint labeli);
		void addParam(IcParamCode p,std::vector<double> args);
		void init(const char* filename,uint labeli);
		void saveArff(const char* filename);
		void setSobelOptions(double sigma,uint size);
		void setLabels(std::vector<std::string> l);
		void reset();
		std::vector<DSItem> getDS();
		bool isShapeSet;
		std::vector<DSItem> ds;

	private:
		double sobelSigma;
		uint sobelSize;
		Sobel* sobel;


		Image mainImg;
		Image edgeImg;
		Image scaledImg;

		Comp* compImgIn;
		Comp* compImgOut;

		std::vector<IcParamCode> params;

		std::vector<std::string> labels;

		// Image newImageFrom(Image* i);
		void loadImageTo(const char* filename,Image *im);
		void ilError(uint code);
		std::vector<double>* currentData;
		uint dsIndex;

		double imgDeltax;
		double imgDeltay;

		void fractdim_shape(Image* i);//TODO test
		void humoments_shape(Image* image);//TODO test

		void fourier_texture(Image* i);
		void netactivity_texture(Image* i);//TODO

		uint fractdim_no;
		double fourier_r;

		uint fourier_maxDistanceToCenter;

};
#endif
