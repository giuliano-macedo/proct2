#include "proct2.hpp";
ImageLoader::ilError(uint code){
	switch(code){
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			fprintf(stderr,"Erro, argumento inválido");
			break;
	}
}
ImageLoader::ImageLoader(){
	//TODO
}
static Image ImageLoader::getImage(const char* filename){
	Image ans;  

	uint error = lodepng_decode_file(&ans.data, &ans.w, &ans.h, filename,LCT_GREY,8);
	if(error){	
		fprintf(stderr,"ERRO LODE  %u: %s\n", error, lodepng_error_text(error));
		fprintf(stderr, "Pulando arquivo:%s\n",filename );
		ans.data=NULL;
	}

  return ans;
}
void ImageLoader::addParam(uint p){
	if(p>IC_MAX_ARG_CODE)ilError(3);
	params[noParams]=p;
	noParams++;
}
void ImageLoader::addImage(const char* filename{
	Image im=getImage(filename);
	if(im.data==NULL)return;

	for(uint i=0;i<noParams;i++){
		switch(params[i]){
			case ICFOURIER_SHAPE:
				fourier_shapex(&im);
				break;
			case ICFRACTDIM_SHAPE:
				fractdim_shape(&im);
				break;
			case ICFOURIER_TEX:
				fourier_tex(&im);
				break;
			case ICFRACTDIM_TEX:
				fractdim_tex(&im);
				break;
		}
	}
}