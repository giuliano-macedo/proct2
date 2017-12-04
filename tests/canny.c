//Deixando sem uso, kmeans não é muito bom com imagens em tons de cinza
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "lodepng.h"
#include<time.h>
typedef struct Image{
    unsigned char* data;
    uint w;
    uint h;
} Image;
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
static void saveImage(Image *i,const char* filename){
    uint s=lodepng_encode_file(filename,i->data, i->w, i->h,LCT_GREY, 8);
    if(s){
        fprintf(stderr, "Falha ao salvar imagem %u\n",s);
        exit(127);
    }
}
unsigned char convoluteAt(Image* im,Image* kernel,int j,int i){
	uint ans=0;

	uint w=im->w;
	uint h=im->h;

	uint kw=kernel->w;
	uint kh=kernel->h;
	
	uint hkw=kw/2; 
	uint hkh=kh/2;


	unsigned char* d=im->data;
	unsigned char* k=kernel->data;

	uint x,y;
	for(y=0;y<kh;y++){
       for(x=0;x<kw;x++){
    		i-=hkw;
    		j-=hkh;
    		if(i<0){
    			i+=w;
    		}
    		else if(i>w){
    			i-=w;
    		}
    		if(j<0){
    			j+=h;
    		}
    		else if(j>h){
    			j-=h;
    		}
    		ans+=k[(y*kw)+x]*d[(j*w)+i];
       }
   }
   return (unsigned char)ans;	
}
void canny(Image in,Image *out){
	unsigned char *im=in.data;
	unsigned char*d=out->data;
	unsigned char temp[9]={0,0,1,
						   0,0,1,
						   0,0,1,
							};
	Image ker={temp,3,3};
	uint w=in.w;
	uint h=in.h;
	uint x,y;
	for(y=0;y<h;y++){
       for(x=0;x<w;x++){
    		d[(y*w)+x]=convoluteAt(&in,&ker,x,y);
       }
   }
}
int main(int argc,char** argv){
    srand(time(0));
    if(argc!=3){
        fprintf(stderr, "[Uso] %s [arquivo.png] [saida.png] \n",argv[0]);
        return -1;
    }
    Image img = getImage(argv[1]);
    Image ans = {(unsigned char*)malloc(img.h*img.w),img.w,img.h};
    canny(img,&ans);
    saveImage(&ans,argv[2]);

}