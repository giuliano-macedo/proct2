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
void ImageLoader::addParam(uint p){
	if(p>IC_MAX_ARG_CODE)ilError(3);
	params[noParams]=p;
	noParams++;
}
void ImageLoader::addImage(char* path{
	png::image< png::gray_pixel> im(path); //TODO CHECK IF ITS ONLY THIS TO DO WHEN OPENING IMAGE
	uint w=im.get_width();
	uint h=im.get_height();

	for(uint i=0;i<noParams;i++){
		switch(params[i]){
			case ICFOURIER_TEX:
				fourier_tex(im,w,h);
				break;
			case ICFRACTDIM_SHAPE:
				fractdim_shape(im,w,h);
				break;
			case ICFOURIER_TEX:
				fourier_tex(im,w,h);
				break;
			case ICFRACTDIM_TEX:
				fractdim_tex(im,w,h);
				break;
		}
	}
}