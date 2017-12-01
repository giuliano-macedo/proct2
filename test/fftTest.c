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
        magVec[i]=m;
        if(m>n){
            n=m;
        }
    }
    n=255/(log(1+n));

    for(i=0;i<s;i++){
        m=magVec[i];
        // printf("%u %.2lf\n",m,((double)m/n));
        d[i]= (unsigned int)n*log((double)m);
    }
}
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
static Image newImage(uint h,uint w){
	Image i={NULL,480,640};
    i.data=(unsigned char*)malloc(i.w*i.h);
    // if(!i.data)ilError(0);
	return i;
}
typedef struct Image Image;
int main(){//IMPLEMENT FTT AND TEST IT HERE

    Image i = getImage("clown.png");
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
