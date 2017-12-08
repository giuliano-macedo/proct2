#include "Sobel.hpp"
Sobel::Sobel(uint iw,uint ih){
	// void* test =(void*) new Convolutioner({NULL,3,3});
	// ((Convolutioner*)test)->setKernel({NULL,3,3});

	w=iw;
	h=ih;
	blurd =(unsigned char*)malloc(h*w);
	img2norm = (double*)malloc(sizeof(double)*w*h);
	blurImg={blurd,w,h};
	blurConv = new Convolutioner(genGaussKernel(1.5,3));
	
	convx = new Convolutioner((Kernel){sx,3,3});
	convy = new Convolutioner((Kernel){sy,3,3});

	convx->setImage(blurImg);
	convy->setImage(blurImg);

}
void Sobel::setBlurOptions(double sigma,uint size){
	blurConv->setKernel(genGaussKernel(sigma,size));
}
void Sobel::run(Image in,Image* out){
	uint x,y;
	blurConv->setImage(in);
	//blur it
	for(y=0;y<h;y++){
		for(x=0;x<w;x++){
			blurd[(y*w)+x]=blurConv->convoluteAt(x,y);
    	}
	}
	double cx,cy;
	double p;
	double m=0;
	//Sobel it
	for(y=0;y<h;y++){
       for(x=0;x<w;x++){
    		cx=convx->convoluteAt(x,y);
			cy=convy->convoluteAt(x,y);
    		p=sqrt((cx*cx)+(cy*cy));
    		if(p>m){
    			m=p;
    		}
    		img2norm[(y*w)+x]=p;
       }
   }
   //Normalize it
   unsigned char* d=out->data;
   m/=255.0;
   for(y=0;y<h;y++){
       for(x=0;x<w;x++){
       		d[(y*w)+x]=img2norm[(y*w)+x]/m;
		}
	}
}