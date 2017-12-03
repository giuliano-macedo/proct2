#include<stdio.h>
#include<stdlib.h>
#include "../lodepng/lodepng.h"
#include <math.h>
#include <vector>
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


#define hypot(x,y) sqrt(SQR(x)+SQR(y))
std::vector<double> complexMag(Comp* ci,uint w,uint h,uint r){
    Comp c;
    std::vector<double> ans;

    uint hw=w/2;
    uint hh=h/2;

    uint r1=0;
    uint r2=r;

    double m;
    uint y,x;
    double s,dis;
    double temp1,temp2;
    // ans.reserve(w/r);
    for(uint i=0;i<(w/r)+1;i++){
        ans.push_back(0);
    }
    for(y=0;y<h;y++){
        for(uint x=0;x<w;x++){
            c=ci[(y*w)+x];
            m=hypot(c.a,c.b);
            if(x<hw&&y<hh){
                temp1=x;
                temp2=y;
            }
            else if(x>=hw&&y<hh){
                temp1=w-x;
                temp2=y;
            }
            else if(x<hw&&y>=hh){
                temp1=x;
                temp2=h-y;
            }
            else{
                temp1=w-x;
                temp2=h-y;
            }
            dis=hypot(temp1,temp2);
            ans[(int)floor(dis/r)+1]+=m;
            
        }
    }
    ans.push_back(s);
    r1+=r;
    r2+=r;
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
        fprintf(stderr, "[Uso] %s [raio] [arquivo.png] \n",argv[0]);
        return -1;
    }
    uint r = atoi(argv[1]);
    
    Image oi = getImage(argv[2]);
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
    std::vector<double> sum = complexMag(icompout,i.w,i.h,r);
    for(uint i=0;i<sum.size();i++){
        printf("[%i] %.2lf\n",i,sum[i]);
    }
}
