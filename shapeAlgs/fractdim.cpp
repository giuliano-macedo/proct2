#include "../proct2.hpp"
uint boxcount(Image *i,uint d){
	uint ans=0;
	uint ow=i->w;
	uint w=im->w/d;
	uint h=im->h/d;
	for(uint i=0;i<d;i++){
		for(uint j=0;j<d;j++){
			for(uint y=0;y<h;y++){
			    for(uint x=0;x<w;x++){
		            p=im[((y+(i*h))*ow)+(x+(j*w))];//not sure
		            if(p)ans++;
			    }
			}
		}
	}
	return ans;
}
ImageLoader::fractdim_shape(Image* im){
	for(uint i=1;i<im->w;i++){
		// currentData.push_back(log(boxcount(im,i))/log(d));
		currentData.push_back(log(boxcount(im,i)));
	}
}
