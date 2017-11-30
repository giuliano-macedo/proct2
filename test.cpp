#include<stdio.h>
#include<stdlib.h>
#include "lodepng/lodepng.h"
#include <math.h>
struct Image{
	unsigned char* data;
	uint w;
	uint h;
};
static void saveImage(Image *i,char* filename){
    uint s=lodepng_encode_file(filename,i->data, i->w, i->h,LCT_GREY, 8);
    if(s){
        fprintf(stderr, "Falha ao salvar imagem %u\n",s);
        exit(127);
    }
}
static Image newImage(uint h,uint w){
	Image i={NULL,480,640};
    i.data=(unsigned char*)malloc(i.w*i.h);
    // if(!i.data)ilError(0);
	return i;
}
typedef struct Image Image;
int main(){//IMPLEMENT FTT AND TEST IT HERE
    Image i = newImage(640,480);
    uint x,y;
    for(y=0;y<i.h;y++){
        for(x=0;x<i.w;x++){
            i.data[(i.w*y)+x]=(uint)122;
        }
    }
    saveImage(&i,"test.png");
}
