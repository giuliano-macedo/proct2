#include "imageUtils.h"
void iuEror(int x){
	switch(x){
		case 0:
			fprintf(stderr, "Falha ao alocar imagem\n");
			exit(127);
		case 1:
			fprintf(stderr, "Falha ao salvar imagem \n");
			exit(127);
	}
}
Image loadimage(const char* filename){
	Image ans;

	uint error = lodepng_decode_file(&ans.data, &ans.w, &ans.h, filename,LCT_GREY,8);
	if(error){
		fprintf(stderr,"Erro ao abrir arquivo %s:%u\n", lodepng_error_text(error),error);
		ans.data=NULL;
	}

  return ans;
}
Image newImageFrom(Image* i){
    Image ans;
	ans.h=i->h;
    ans.w=i->w;

    ans.data=(unsigned char*)malloc(i->w*i->h);
    if(!ans.data)iuEror(0);
    return ans;
}
Image newImage(uint h,uint w){
	Image ans={NULL,480,640};
    ans.data=(unsigned char*)malloc(w*h);
    if(!ans.data)iuEror(0);
	return ans;
}
int saveImage(Image *i,char* filename){
	uint error=lodepng_encode_file(filename,i->data, i->w, i->h,LCT_GREY, 8);
	if(error){
		fprintf(stderr,"Erro ao salvar arquivo %s:%u\n", lodepng_error_text(error),error);
		return 1;
	}
	return 0;
}
void scaleImageLIN(Image in,Image* out,float dx,float dy){
    unsigned char* d =out->data;
    uint w=out->w;
    uint h=out->h;
    
    uint ow=in.w;
    uint oh=in.h;
    unsigned char* im=in.data;
    unsigned char p1,p2,p3,p4;

    double wf,hf,d1,d2,d3,d4;
    for(uint y=0;y<h;y++){
        for(uint x=0;x<w;x++){
            wf=((double)x*ow)/w;
            hf=((double)y*oh)/h;

            wf=wf-floor(wf);
            hf=hf-floor(hf);

            d1=(1-wf) * (1-hf);
            d2= wf    * (1-hf);
            d3=(1-wf) * hf;
            d4= wf    * hf;

            p1=im[((int)(y/dy))*ow+(int)(x/dx)];
            if(x+1<w && y+1<h){
                p2=im[((int)(y/dy))*ow+(int)((x)/dx)+1];
                p3=im[((int)((y)/dy)+1)*ow+(int)(x/dx)];
                p4=im[((int)((y)/dy)+1)*ow+(int)(x/dx)+1];
            }
            else if(x+1<w){
                p2=im[((int)(y/dy))*ow+(int)((x)/dx)+1];
                p3=im[(int)(x/dy)];
                p4=im[(int)(x/dy)+1];
            }
            else if(y+1<h){
                p2=im[((int)(y/dy))*ow];
                p3=im[((int)((y)/dy)+1)*ow+(int)(x/dx)];
                p4=im[((int)((y)/dy)+1)*ow];
            }
            else{
                p2=im[(int)(1/dy)];
                p3=im[((int)(1/dy))*ow];
                p4=im[((int)(1/dy))*ow+(int)(1/dx)];
            }

            d[(y*w)+x]=(p1*d1)+(p2*d2)+(p3*d3)+(p4*d4);
        }
    }
}