#include "ImageLoader.hpp"
uint boxcount(Image *im,uint d){//probably wrong
	uint ans=0;
	unsigned char *data=im->data;
	uint p;
	uint ow=im->w;
	uint w=im->w/d;
	uint h=im->h/d;
	uint i,j,y,x;
	for(i=0;i<d;i++){
		for(j=0;j<d;j++){
			for(y=0;y<h;y++){
			    for(x=0;x<w;x++){
		            p=data[((y+(i*w))*ow)+(x+(j*w))];
		            if(p){
		            	ans++;
		            	y=h;
		            	x=w;
		            }
			    }
			}
			
		}
	}
	return ans;
}
void ImageLoader::fractdim_shape(Image* im){
	uint m=(im->w>im->h)?im->h:im->w;
	if(fractdim_no>m)ilError(3);
	
	for(uint i=m;i>=(m-fractdim_no);i--){
		currentData->push_back((boxcount(im,i)));
	}
}
