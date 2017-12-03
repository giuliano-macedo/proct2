#include "ImageLoader.hpp"
uint boxcount(Image *im,uint d){
	uint ans=0;
	unsigned char *data=im->data;
	uint p;
	uint ow=im->w;
	uint w=im->w/d;
	uint h=im->h/d;
	for(uint i=0;i<d;i++){
		for(uint j=0;j<d;j++){
			for(uint y=0;y<h;y++){
			    for(uint x=0;x<w;x++){
		            p=data[((y+(i*w))*ow)+(x+(j*w))];//not sure
		            if(p)ans++;
			    }
			}
		}
	}
	return ans;
}
void ImageLoader::fractdim_shape(Image* im){
	for(uint i=1;i<im->w;i++){//TODO maybe d will devide too much w or h
		// currentData.push_back(log(boxcount(im,i))/log(d));
		currentData.push_back(log(boxcount(im,i)));
	}
}
