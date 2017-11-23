#include "proct2.hpp";
ImageCategorizer::ImageCategorizer(){
	//TODO
}
void ImageCategorizer::addParam(){

}
void ImageCategorizer::addImage(char* path{
	png::image< png::gray_pixel> im(path); //TODO CHECK IF ITS ONLY THIS TO DO WHEN OPENING IMAGE
	uint w=im.get_width();
	uint h=im.get_height();

	for(uint i=0;i<noParams;i++){
		switch(params[i]){
			case FOURIER_TEX:
				fourier_tex(im,w,h);
				break;
			case FRACTDIM_SHAPE:
				fractdim_shape(im,w,h);
				break;
			case FOURIER_TEX:
				fourier_tex(im,w,h);
				break;
			case FRACTDIM_TEX:
				fractdim_tex(im,w,h);
				break;
		}
	}
}