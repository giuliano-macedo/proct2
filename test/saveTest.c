#include<stdio.h>
#include<stdlib.h>
#include "../lodepng/lodepng.h"

typedef struct Image{
	unsigned char* data;
	uint w;
	uint h;
} Image;
static void saveImage(Image *i,char* filename){
    uint s=lodepng_encode_file(filename,i->data, i->w, i->h,LCT_GREY, 8);
    if(s){
        fprintf(stderr, "Falha ao salvar imagem %u\n",s);
        exit(127);
    }
}
Image getImage(const char* filename){
    Image ans;

    uint error = lodepng_decode_file(&ans.data, &ans.w, &ans.h, filename,LCT_GREY,8);
    if(error){
        fprintf(stderr,"LODE: ERRO %u %s\n", error, lodepng_error_text(error));
        fprintf(stderr, "Pulando arquivo:%s\n",filename );
        ans.data=NULL;
    }

  return ans;
}
int main(){
    Image i = getImage("clown.png");
    saveImage(&i,"clownTest.png");
}
