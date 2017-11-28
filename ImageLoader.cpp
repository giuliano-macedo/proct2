#include "proct2.hpp";
ImageLoader::ilError(uint code){
	switch(code){
		case 0:
			fprintf(stderr,"ImageLoader:Erro, não foi possível alocar memória para imagem\n");
			exit(127);
			break;
		case 1:
			break;
		case 2:
			fprintf(stderr,"ImageLoader:Erro, argumento de limiarização não definido\n");
			break;
		case 3:
			fprintf(stderr,"ImageLoader:Erro, argumento inválido\n");
			exit(127);
		case 4:
			fprintf(stderr,"ImageLoader:Erro, impossível realizar mesma função duas vezes\n");
			exit(127);
	}
}
ImageLoader::ImageLoader(){
	isShapeSet=false;
	isThreshSet=false;
	dsIndex=0;
}
void ImageLoader::addParam(IcParamCode p,int* args){
	paramArgs.push_back(args);
	size_t noParams=params.size();
	if(p>IC_MAX_ARG_CODE)ilError(3);
	for(uint i=0;i<noParams;i++)if(params[i]==p)ilError(4);
	if(p<=ICHUMOMENTS_SHA){//if param is shape
		isShapeSet=true;
	}
	params.push_back(p);
}
void ImageLoader::setThresh(ThreshCode p,int* args){
	isThreshSet=true;
	tparam=p;
	targs=args;
}
void ImageLoader::addImage(const char* filename{
	size_t noParams=params.size();
	ds->push_back(NULL);//Not sure
	currentData=ds[dsIndex].data;
	if(isShapeSet&&!isThreshSet)ilError(2);
	Image im,tim,eim;
	im=getImage(filename);
	if(im.data==NULL)return;
	if(isThreshSet){
		switch(tparam){
			case ICSIMPLE_THR:
				tim=simple_threshold(&im);
				break;
			case ICKMEAN_THR:
				tim=kmean_threshold(&im);
				break;
		}
		eim=binEdgeDetect(&tim);
	}

	for(uint i=0;i<noParams;i++){
		switch(params[i]){
			case ICFOURIER_SHA:
				fourier_shape(&tim);
				break;
			case ICFRACTDIM_SHA:
				fractdim_shape(&tim);
				break;
			case ICHUMOMENTS_SHA:
				humoments_shape(&tim);
				break;
			case ICFOURIER_TEX:
				fourier_texture(&im);
				break;
			case ICFRACTDIM_TEX:
				fractdim_texture(&im);
				break;
			case ICLBP_TEX:
				lbp_texture(&im);
				break;
		}
	}
	dsIndex++;
}
