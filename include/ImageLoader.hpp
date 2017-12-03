#include <stdio.h>
#include "lodepng.h"
#include "fft.h"
#include "imageUtils.h"
#include <math.h>
#include <vector>
enum IcParamCode{
	ILFOURIER_SHA=1,
	ILFRACTDIM_SHA,
	ILHUMOMENTS_SHA,

	ILFOURIER_TEX,
	ILFRACTDIMKMEANS_TEX,

	IL_NO_ARGTYPES
};
enum ThreshCode{
	ILSIMPLE_THR=IL_NO_ARGTYPES,
	ILKMEAN_THR
};

struct dataSetItem{
	std::vector<double> data;
	uint label;
};

typedef struct dataSetItem DSItem;
typedef unsigned int uint;
class ImageLoader{
	public:
		ImageLoader();
		void addImage(const char* filename);
		void setThresh(ThreshCode p,std::vector<int> args);
		void addParam(IcParamCode p,std::vector<int> args);
		void init(const char* filename);
		void saveArff(const char* filename);
		DSItem* getDS();
		bool isShapeSet;
		bool isThreshSet;

	private:
		Image mainImg;
		Image thresholdImg;
		Image edgeImg;
		Image scaledImg;

		Comp* compImgIn;
		Comp* compImgOut;


		ThreshCode tparam;
		std::vector<int> targs;

		std::vector<IcParamCode> params;
		std::vector<std::vector<int>> paramArgs;

		Image newImageFrom(Image* i);
		void loadImageTo(const char* filename,Image *im);
		void ilError(uint code);
		std::vector<DSItem> ds;
		std::vector<double> currentData;
		uint dsIndex;

		uint imgDeltax;
		uint imgDeltay;

		Image simple_threshold(Image* i);
		Image kmean_threshold(Image* i);
		Image binEdgeDetect(Image* i);

		void fourier_shape(Image* i);
		void fractdim_shape(Image* i);
		void humoments_shape(Image* image);

		void fourier_texture(Image* i);
		void fractdimkmeans_texture(Image* i);

};
