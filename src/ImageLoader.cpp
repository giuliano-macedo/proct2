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
			fprintf(stderr,"Erro ao processar primeira imagem\n");
			exit(127);
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
	fourier_r=0;
}
void ImageLoader::setSobelOptions(double sigma,uint size){
	sobelSigma=sigma;
	sobelSize=size;
}
void ImageLoader::reset(){
	ds.clear();
	dsIndex=0;
}
void ImageLoader::init(const char* filename,uint labeli){
	mainImg = loadImage(filename);
	if(!mainImg.data)ilError(2);
	sobel=new Sobel(mainImg.w,mainImg.h);
	if(sobelSigma){
		sobel->setBlurOptions(sobelSigma,sobelSize);
	}
	edgeImg=mainImg;
	scaledImg=mainImg;

	uint s=mainImg.w*mainImg.h;

	imgDeltax=(1<<(int)(ceil(log2(mainImg.w))))/(double)mainImg.w;//2^ceil(log2(w))
    imgDeltay=(1<<(int)(ceil(log2(mainImg.h))))/(double)mainImg.h;//2^ceil(log2(h))

    scaledImg.w=(int)ceil(imgDeltax*mainImg.w);
    scaledImg.h=(int)ceil(imgDeltay*mainImg.h);
	edgeImg.data=	  (unsigned char*)malloc(s);
	scaledImg.data=	  (unsigned char*)malloc((int)ceil(s*imgDeltax*imgDeltay));

	compImgIn=(Comp*)malloc(sizeof(Comp)*(int)ceil(s*imgDeltax*imgDeltay));
	compImgOut=(Comp*)malloc(sizeof(Comp)*(int)ceil(s*imgDeltax*imgDeltay));

	if(
		!mainImg.data||
		!edgeImg.data||
		!scaledImg.data||
		!compImgOut||
		!compImgIn){
		ilError(0);
	}
	if(fourier_r){//if fourier_texture
		uint hw,hh;
		hw=scaledImg.w/2;
		hh=scaledImg.h/2;
		fourier_maxDistanceToCenter=ceil(sqrt((hw*hw)+(hh*hh))/fourier_r);
	}
	processImage(filename,labeli);

}
void ImageLoader::loadImageTo(const char* filename,Image *im){
	free(im->data);
	uint w,h;
	uint error = lodepng_decode_file(&im->data, &w, &h, filename,LCT_GREY,8);
	if(error){
		fprintf(stderr,"Erro ao abrir arquivo %s : %s\n",filename,lodepng_error_text(error));
		im->data=NULL;
		return;
	}
	if(w!=im->w||h!=im->h)ilError(1);

}
void ImageLoader::addParam(IcParamCode p,std::vector<double> args){
	switch(p){
		case ILFRACTDIM_SHA:
			if(args.size()!=1)ilError(5);
			fractdim_no=args[0]+1;
			break;
		case ILHUMOMENTS_SHA:
			if(args.size()!=0)ilError(5);
			break;
		case ILFOURIER_TEX:
			if(args.size()!=1)ilError(5);
			fourier_r=args[0];
			if(fourier_r<=0)ilError(3);
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
void ImageLoader::setLabels(std::vector<std::string> l){
	labels=l;
}
void ImageLoader::saveArff(const char* filename){
	uint i;
	FILE* f=fopen(filename,"w");
	if(!f){
		fprintf(stderr,"Falha ao salvar arquivo arff\n");
		exit(-1);
	}
	uint s;
	uint ss;
	DSItem ad;
	fprintf(f,"@RELATION \"Trabalho 2 TPI\"\n");
	s=ds[0].data.size();
	for(i=0;i<s;i++){
		fprintf(f, "@ATTRIBUTE value%u NUMERIC\n",i );
	}
	fprintf(f,"@ATTRIBUTE class {");
	s=labels.size();
	for(i=0;i<s;i++){
		fprintf(f, "%s",labels[i].c_str());
		if(i!=s-1){
			fprintf(f, ",");
		}
	}
	fprintf(f, "}\n");

	fprintf(f,"@DATA\n");
	s=ds.size();
	for(i=0;i<s;i++){
		ad=ds[i];
		ss=ad.data.size();
		for(uint j=0;j<ss;j++){
			fprintf(f,"%e,",ad.data[j]);
			// fprintf(f,"%.25lf,",ad.data[j]);
		}
		fprintf(f,"%s\n",labels[ad.label].c_str());
	}
}
std::vector<DSItem> ImageLoader::getDS(){
	return ds;
}
void ImageLoader::processImage(const char* filename,uint labeli){
	size_t noParams=params.size();
	ds.push_back({std::vector<double>(),labeli});
	currentData=&(ds[dsIndex].data);
	loadImageTo(filename,&mainImg);
	if(mainImg.data==NULL){
		return;
	}
	if(isShapeSet){
		sobel->run(mainImg,&edgeImg);
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
				fourier_texture(&mainImg);
				break;
			case ILNETACTIVITY_TEX:
				netactivity_texture(&mainImg);
				break;
		}
	}
	// printf("currentData:\n");
	// for(int i=0;i<currentData->size();i++){
	// 	printf("%-10.20lf\n",currentData[0][i]);
	//
	dsIndex++;
}
