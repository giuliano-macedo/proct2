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
			fprintf(stderr,"ImageLoader:Erro, quantidade de argumentos invalida para procesamento da imagem\n");
			exit(127);
	}
}
ImageLoader::ImageLoader(){
	isShapeSet=false;
	dsIndex=0;
	sobelSigma=0;
}
void ImageLoader::setSobelOptions(double sigma,uint size){
	sobelSigma=sigma;
	sobelSize=size;
}
void ImageLoader::init(const char* filename){
	mainImg = loadImage(filename);
	sobel=new Sobel(mainImg.w,mainImg.h);
	if(sobelSigma){
		sobel->setBlurOptions(sobelSigma,sobelSize);
	}
	edgeImg=mainImg;
	scaledImg=mainImg;

	uint s=mainImg.w*mainImg.h;

	imgDeltax=(1<<(int)(ceil(log2(mainImg.w))))/(double)mainImg.w;//2^ceil(log2(w))
    imgDeltay=(1<<(int)(ceil(log2(mainImg.h))))/(double)mainImg.h;//2^ceil(log2(h))

	edgeImg.data=	  (unsigned char*)malloc(s);
	scaledImg.data=	  (unsigned char*)malloc((int)ceil(s*imgDeltax*imgDeltay));

	compImgIn=(Comp*)malloc(sizeof(Comp)*s);
	compImgOut=(Comp*)malloc(sizeof(Comp)*s);

	if(
		!mainImg.data||
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
void ImageLoader::addParam(IcParamCode p,std::vector<double> args){
	switch(p){
		case ILFRACTDIM_SHA:
			if(args.size()!=1)ilError(5);
			fractdim_no=args[0];
			break;
		case ILHUMOMENTS_SHA:
			if(args.size()!=0)ilError(5);
			break;
		case ILFOURIER_TEX:
			if(args.size()!=1)ilError(5);
			fourier_r=args[0];
			break;
		case ILNETACTIVITY_TEX:
			//TODO
			break;
	}
	size_t noParams=params.size();
	if(p>IL_NO_ARGTYPES)ilError(3);
	for(uint i=0;i<noParams;i++)if(params[i]==p)ilError(4);
	if(p<=ILHUMOMENTS_SHA){//if param is shape
		isShapeSet=true;
	}
	params.push_back(p);
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
	if(isShapeSet){
		sobel->run(im,&edgeImg);
	}

	for(uint i=0;i<noParams;i++){
		switch(params[i]){
			case ILFRACTDIM_SHA:
				fractdim_shape(&edgeImg);
				break;
			case ILHUMOMENTS_SHA:
				humoments_shape(&edgeImg);
				break;
			case ILFOURIER_TEX:
				fourier_texture(&im);
				break;
			case ILNETACTIVITY_TEX:
				netactivity_texture(&im);
				break;
		}
	}
	dsIndex++;
}
