#include<stdio.h>
#include<stdlib.h>
#include "../lodepng/lodepng.h"
#include <math.h>
#include "../fft/fft.h"

typedef struct Image{
	unsigned char* data;
	uint w;
	uint h;
} Image;

Comp* imageToComp(Image im,Comp* ci){
    uint s=im.w*im.h;
    unsigned char* d = im.data;
    uint i;
    Comp c={0,0};
    for(i=0;i<s;i++){
        c.a=(double)d[i];
        ci[i]=c;
    }
    return ci;
} 

void complexMag2Image(Comp* ci,Image im,unsigned* magVec){
    Comp c;
    uint i,x,y,w=im.w,h=im.h,s=w*h,m;
    uint hw=w/2,hh=h/2;
    unsigned char* d = im.data;
    unsigned n=0;
    for(i=0;i<s;i++){
        c=ci[i];
        m=sqrt(SQR(c.a)+SQR(c.b));
        x=i%w;
        y=i/w;
        if(x<hw&&y<hh){//I
            x=hw-1-x;
            y=hh-1-y;
            magVec[(y*w)+x]=m;
        }
        else if(x>=hw&&y<hh){//II
            x=hw-1-x;
            y=hh-y;
            magVec[(y*w)+x]=m;
        }
        else if(x<hw&&y>=hh){//III
            x=hw-1-x;
            y=-y-1+hh+h;
            magVec[(y*w)+x]=m;
        }
        else{//IV
            x=hw-1-x;
            y=-y+hh+h;
            magVec[(y*w)+x]=m;
        }
        // magVec[i]=m;
        if(m>n){
            n=m;
        }
    }
    n=255/(log(1+n));

    for(i=0;i<s;i++){
        m=magVec[i];
        // printf("%u %.2lf\n",m,((double)m/n));
        d[i]= (unsigned int)n*log(1+(double)m);
    }
}
static void saveImage(Image *i,const char* filename){
    uint s=lodepng_encode_file(filename,i->data, i->w, i->h,LCT_GREY, 8);
    if(s){
        fprintf(stderr, "Falha ao salvar imagem %u\n",s);
        exit(127);
    }
}
static Image scaleImageNNS(Image in,float dx,float dy){
    Image ans;
    unsigned char* d=(unsigned char*)malloc((int)ceil(in.w*in.h*dx*dy));
    uint w=(int)ceil(in.w*dx);
    uint h=(int)ceil(in.h*dy);
    uint ow=in.w;
    unsigned char* im=in.data;
    unsigned char p;
    
    for(uint y=0;y<h;y++){
        for(uint x=0;x<w;x++){
            p=im[((int)(y/dy))*ow+(int)(x/dx)];
            d[(y*w)+x]=p;
        }
    }
    ans.w=w;
    ans.h=h;
    ans.data=d;
    return ans;
    
}
    
static Image scaleImageLIN(Image in,float dx,float dy){
    Image ans;
    unsigned char* d=(unsigned char*)malloc((int)ceil(in.w*in.h*dx*dy));
    uint w=(int)ceil(in.w*dx);
    uint h=(int)ceil(in.h*dy);
    uint ow=in.w,oh=in.h;
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
    ans.w=w;
    ans.h=h;
    ans.data=d;
    return ans;
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
int main(int argc,char** argv){
    if(argc!=3){
        fprintf(stderr, "[Uso] %s [arquivo.png] [saida.png]\n",argv[0]);
        return -1;
    }
    
    Image oi = getImage(argv[1]);
    if(oi.data==NULL)return -1;
    double deltax=(1<<(int)(ceil(log2(oi.w))))/(double)oi.w;//2^ceil(log2(w))
    double deltay=(1<<(int)(ceil(log2(oi.h))))/(double)oi.h;//2^ceil(log2(h))


    Image i=scaleImageLIN(oi,deltax,deltay);

    Comp* icompin = (Comp*)malloc(sizeof(Comp)*i.w*i.h);
    Comp* icompout = (Comp*)malloc(sizeof(Comp)*i.w*i.h);
    if(!icompin||!icompout){
        fprintf(stderr,"Erro ao alocar memória para imagem complexa\n");
        exit(127);
    }
    printf("i malloc ok\n");
    imageToComp(i,icompin);
    printf("i2c ok\n");
    fft(icompin,icompout,1,i.w*i.h,0);
    printf("fft ok\n");
    complexMag2Image(icompout,i,(unsigned*)icompin);
    printf("cm2i ok\n");
    saveImage(&i, argv[2]);
}
