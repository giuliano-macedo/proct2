#include "proct2.hpp"
void ImageLoader::ilError(uint code){
	switch(code){
		case 0:
			fprintf(stderr,"ImageLoader:Erro, não foi possível alocar memória para imagens\n");
			exit(127);
			break;
		case 1:
			fprintf(stderr,"ImageLoader:Erro, imagem com dimensões dos outros\n");
			exit(127);
			break;
		case 2:
			break;
		case 3:
			fprintf(stderr,"ImageLoader:Erro, argumento inválido\n");
			exit(127);
		case 4:
			fprintf(stderr,"ImageLoader:Erro, impossível realizar mesma função duas vezes\n");
			exit(127);
		case 5:
			fprintf(stderr,"ImageLoader:Erro, somente uma função de limiar possível\n");
			exit(127);
	}
}
ImageLoader::ImageLoader(){
	isShapeSet=false;
	isThreshSet=false;
	dsIndex=0;
}
void ImageLoader::init(const char* filename){
	mainImg = loadImage(filename);
	thresholdImg=mainImg;
	edgeImg=mainImg;
	scaledImg=mainImg;

	uint s=mainImg.w*mainImg.h;

	imgDeltax=(1<<(int)(ceil(log2(mainImg.w))))/(double)mainImg.w;//2^ceil(log2(w))
    imgDeltay=(1<<(int)(ceil(log2(mainImg.h))))/(double)mainImg.h;//2^ceil(log2(h))

	thresholdImg.data=(unsigned char*)malloc(s);
	edgeImg.data=	  (unsigned char*)malloc(s);
	scaledImg.data=	  (unsigned char*)malloc((int)ceil(s*imgDeltax*imgDeltay));

	compImgIn=(Comp*)malloc(sizeof(Comp)*s);
	compImgOut=(Comp*)malloc(sizeof(Comp)*s);

	if(
		!mainImg.data||
		!thresholdImg.data||
		!edgeImg.data||
		!scaledImg.data||
		!compImgOut||
		!compImgIn){
		ilError(0);
	}
	addImage(filename);

}
void ImageLoader::loadImageTo(const char* filename,Image *im){
	free(im->data);
	uint w,h;
	uint error = lodepng_decode_file(&im->data, &w, &h, filename,LCT_GREY,8);
	if(w!=im->w||h!=im->h)ilError(1);
	if(error){
		fprintf(stderr,"Erro ao abrir arquivo %s : %s\n",filename,lodepng_error_text(error));
		im->data=NULL;
	}
}
void ImageLoader::addParam(IcParamCode p,std::vector<int> args){
	paramArgs.push_back(args);
	size_t noParams=params.size();
	if(p>IL_NO_ARGTYPES)ilError(3);
	for(uint i=0;i<noParams;i++)if(params[i]==p)ilError(4);
	if(p<=ILHUMOMENTS_SHA){//if param is shape
		isShapeSet=true;
	}
	params.push_back(p);
}
void ImageLoader::setThresh(ThreshCode p,std::vector<int> args){
	if(isThreshSet)ilError(5);
	isThreshSet=true;
	tparam=p;
	targs=args;
}
void ImageLoader::saveArff(const char* filename){
	FILE* f=fopen(filename,"w");
	if(!f){
		fprintf(stderr,"Falha ao salvar arquivo arff\n");
		exit(-1);
	}
	uint s=ds.size();
	uint ss;
	DSItem ad;
	
	//TODO:header
	fprintf(f,"@DATA\n");
	for(uint i=0;i<s;i++){
		ad=ds[i];
		ss=ad.data.size();
		for(uint j=0;j<ss;j++){
			fprintf(f,"%lf,",ad.data[i]);
		}
		fprintf(f,"%u\n",ad.label);
	}
}
std::vector<DSItem> ImageLoader::getDS(){
	return ds;
}
void ImageLoader::addImage(const char* filename){
	size_t noParams=params.size();
	ds.push_back({std::vector<double>(),0});//Not sure
	currentData=ds[dsIndex].data;
	Image im,tim;
	loadImageTo(filename,&im);
	if(im.data==NULL){
		printf("Pulando arquivo:%s\n",filename );
		return;
	}
	if(isThreshSet){
		switch(tparam){
			case ILSIMPLE_THR:
				simple_threshold(&im);
				break;
			case ILKMEAN_THR:
				kmean_threshold(&im);
				break;
		}
		binEdgeDetect(&tim);
	}

	for(uint i=0;i<noParams;i++){
		switch(params[i]){
			case ILFOURIER_SHA:
				fourier_shape(&tim);
				break;
			case ILFRACTDIM_SHA:
				fractdim_shape(&tim);
				break;
			case ILHUMOMENTS_SHA:
				humoments_shape(&tim);
				break;
			case ILFOURIER_TEX:
				fourier_texture(&im);
				break;
			case ILFRACTDIMKMEANS_TEX:
				fractdimkmeans_texture(&im);
				break;
		}
	}
	dsIndex++;
}
