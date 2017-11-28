#include "../proct2.hpp"
Image simple_threshold(Image* i){
    Image ans=newImageFrom(i);
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
    for(uint y=0;y<h;y++){
	    for(uint x=0;x<w;x++){
            index=(y*w)+x;
            p=im[index];
            ans[index]=((p<t)!=sign)?1:0;
	    }
	}
    return ans;
}
