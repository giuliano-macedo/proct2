#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "lodepng.h"
#include<time.h>
#define PI 3.141592653589793
typedef struct Image{
    unsigned char* data;
    uint w;
    uint h;
} Image;
typedef struct Kernel{
	double* data;
	uint w;
	uint h;
} Kernel;
void printKer(Kernel ker){
	for(uint i=0;i<ker.w*ker.h;i++){
		printf("%lf,",ker.data[i]);
		if(!((i+1)%ker.w)){
			putchar('\n');
		}
	}
}
void normalizeKernel(Kernel *k){
	uint i;
	double s;
	double* ker=k->data;
	uint ks=k->w*k->h;
	for(i=0;i<ks;i++){
		s+=ker[i];
	}
	for(i=0;i<ks;i++){
		ker[i]/=s;
	}
}
Kernel genGaussKernel(uint size,double s){
	double *d =(double*)malloc(size*size*sizeof(double));
	Kernel ans={d,size,size};
	s*=s;
	int x,y;
	uint halfSize=size/2;
	for(uint i=0;i<size;i++){
		for(uint j=0;j<size;j++){
			x=i-halfSize;
			y=j-halfSize;
			d[(i*size)+j]=1.0/(2*PI*s*exp( ( (x*x) + (y*y) )/(2*s) ) ); 

		}
	}
	normalizeKernel(&ans);
	return ans;
}
class Convolutioner{
private:
	double* ker;
	uint kw;
	uint kh;
	uint hkw;
	uint hkh;
	uint ks;

	unsigned char* img;
	uint imgw;
	uint imgh;
	uint imgs;

public:
	Convolutioner(Image in,Kernel k){
		img=in.data;
		imgw=in.w;
		imgh=in.h;
		imgs=imgw*imgh;

		ker=k.data;
		kw=k.w;
		kh=k.h;

		hkw=kw/2;
		hkh=kh/2;
		ks=kw*kh;

	}
	void setImage(Image in){
		img=in.data;
		imgw=in.w;
		imgh=in.h;
	}
	void setKernel(Kernel k){
		ker=k.data;
		kw=k.w;
		kh=k.h;
	}
	double convoluteAt(uint i,uint j){
		double ans=0;
	   	uint x,y;
   		int px,py;
   		for(y=0;y<kh;y++){
			py=j-hkh+y;
			if(py<0){
				py+=hkh;
			}
			else if((j+hkh+y)>imgh-1){
				py-=hkh;
			}
			for(x=0;x<kw;x++){
				px=i-hkw+x;
				if(px<0){
					px+=hkw;
				}
				else if((i+hkw+x)>imgw-1) {
					px-=hkw;
				}
		   		ans+=(int)(ker[(y*kw)+x]*img[(py*imgw)+px]);
			}
		}
		
		//otimizado
		// uint m=(j*imgw)+i;
		// for(y=0;y<ks;y++){

		// 	if(m>imgs-1){
		// 		m-=hkw;
		// 	}
		// 	ans+=(ker[y]*img[m]);
		// 	m++;
		// }
   		return ans;	
	}
};
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

void sobel(Image in,Image *out){
	// unsigned char *im=in.data;
	uint w=in.w;
	uint h=in.h;
	
	uint x,y;

	unsigned char* blurd =(unsigned char*)malloc(in.h*in.w);
	Image blurImg={blurd,in.w,in.h};
	Convolutioner* blurConv = new Convolutioner(in,genGaussKernel(3,2));
    for(y=0;y<h;y++){
       for(x=0;x<w;x++){
    		blurd[(y*w)+x]=(int)blurConv->convoluteAt(x,y);
    	}
	}
	
	unsigned char*d=out->data;
	double temp1[9]={1,0,-1,
				    2,0,-2,
				    1,0,-1
				};
	Convolutioner * convX = new Convolutioner(blurImg,{temp1,3,3});
	double temp2[9]={1,2,1,
				    0,0,0,
				    -1,-2,-1
				};
	Convolutioner * convY = new Convolutioner(blurImg,{temp2,3,3});
	double cx,cy;

	double p;
	double* img2norm = (double*)malloc(sizeof(double)*w*h);
	double m=0;
	for(y=0;y<h;y++){
       for(x=0;x<w;x++){
    		cx=convX->convoluteAt(x,y);
			cy=convY->convoluteAt(x,y);
    		p=sqrt((cx*cx)+(cy*cy));
    		if(p>m){
    			m=p;
    		}
    		img2norm[(y*w)+x]=p;
       }
   }
   m/=255.0;
   // m=1;
   for(y=0;y<h;y++){
       for(x=0;x<w;x++){
       		d[(y*w)+x]=img2norm[(y*w)+x]/m;
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
    sobel(img,&ans);
    saveImage(&ans,argv[2]);

}