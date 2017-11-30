#include <stdio.h>
#include "lode/lode.h"
#include <math.h>
#include <vector>
enum ThreshCode{
	ICSIMPLE_THR,
	ICKMEAN_THR
};
enum IcParamCode{
	ICFOURIER_SHA,
	ICFRACTDIM_SHA,
	ICHUMOMENTS_SHA,

	ICFOURIER_TEX,
	ICFRACTDIM_TEX,
	ICLBP_TEX,

	iC_NO_ARGTYPES
};

struct dataSetItem{
	std::vector<double> data;
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
		void setThresh(ThreshCode p,int* args);
		void addParam(IcParamCode p,int* args);
		DSItem* getDS();

	private:
		bool isShapeSet;
		bool isThreshSet;

		ThreshCode tparam;
		int* targs;

		std::vector<IcParamCode> params;
		std::vector<int*> paramArgs;


		static Image getImage(const char* filename);
		static Image newImageFrom(Image* i);
		std::vector<DSItem> ds;
		std::vector<double> currentData;
		uint dsIndex;

		Image simple_threshold(Image* i);
		Image kmean_threshold(Image* i);
		Image binEdgeDetect(Image* i);

		void fourier_shape(Image* i);
		void fractdim_shape(Image* i);
		void humoments_shape(Image* image);

		void fourier_texture(Image* i);
		void fractdim_texture(Image* i);
		void lbp_texture(Image* i);

}
