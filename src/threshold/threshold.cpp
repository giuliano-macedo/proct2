#include "proct2.hpp"
Image ImageLoader::simple_threshold(Image* i){
    Image ans=newImageFrom(i);

    uint h=i->h;
    uint w=i->w;
    unsigned char* im =i->data,*ansd=ans.data;
    bool sign;
    int t=targs[0];
    if(t<0){
        t*=-1;
        sign=false;
    }
    else{
        sign=true;
    }
    uint index;
    int p;
    for(uint y=0;y<h;y++){
	    for(uint x=0;x<w;x++){
            index=(y*w)+x;
            p=im[index];
            ansd[index]=((p<t)!=sign)?1:0;
	    }
	}
    return ans;
}
