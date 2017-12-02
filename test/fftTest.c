#include<stdio.h>
#include<stdlib.h>
#include "../lodepng/lodepng.h"
#include <math.h>
typedef struct Comp {
    /* comp of the form: a + bi */
    double a, b;
} Comp;

Comp comp_euler(double x) {
    Comp res;
    res.a = cos(x);
    res.b = sin(x);
    return res;
}

#define PI 3.141592653589793
#define SQR(x) ((x) * (x))
#define comp_mul_self(c, c2) \
do { \
    double ca = c->a; \
    c->a = ca * c2->a - c->b * c2->b; \
    c->b = c->b * c2->a + ca * c2->b; \
} while (0)
void fft(const Comp *sig, Comp *f, int s, int n, int inv) { //sig = input signal, f = output frequency, s = ? , n =length, inv = bool inverse
    int i, hn = n >> 1;//n>>1 = n/2
    Comp ep = comp_euler((inv ? PI : -PI) / (double)hn), ei;
    Comp *pi = &ei, *pp = &ep;
    if (!hn) *f = *sig;
    else
    {
        fft(sig, f, s << 1, hn, inv);
        fft(sig + s, f + hn, s << 1, hn, inv); //s<<1 = s*2
        pi->a = 1;
        pi->b = 0;
        for (i = 0; i < hn; i++)
        {
            Comp even = f[i], *pe = f + i, *po = pe + hn;
            comp_mul_self(po, pi);
            pe->a += po->a;
            pe->b += po->b;
            po->a = even.a - po->a;
            po->b = even.b - po->b;
            comp_mul_self(pi, pp);
        }
    }
}
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
#define lerp(a,b,dx,i) a+(i*(b-a)/dx)//TODO CHECK IF WORKS
static Image scaleImageLIN(Image in,float delta){
    Image ans;
    unsigned char* d=(unsigned char*)malloc((int)ceil(in.w*in.h*delta*delta));
    uint w=(int)ceil(in.w*delta);
    uint h=(int)ceil(in.h*delta);
    uint ow=in.w;
    unsigned char* im=in.data;
    unsigned char p1,p2;
    for(uint y=0;y<h;y++){
        for(uint x=0;x<w;x++){
        p1=im[((int)(y/delta))*ow+(int)(x/delta)];
        if(x+1<w){
            p2=im[((int)(y/delta))*ow+(int)(x+1/delta)];
        }
        else{
            p2=im[((int)(y/delta))*ow];
        }
        d[(y*w)+x]=lerp(p1,p2,delta,(x%delta));
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
typedef struct Image Image;
int main(int argc,char** argv){
    if(argc!=2)return -1;
    Image i=getImage(argv[1]);
    
    // Image oi = getImage(argv[1]);
    // if(oi.data==NULL)return -1;
    // double delt=256/178.0;
    // Image i=scaleImageNNS(oi,delt,delt);

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
    saveImage(&i,"test.png");
}
