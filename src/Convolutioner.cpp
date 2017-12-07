#include "Convolutioner.hpp"
Convolutioner::Convolutioner(Kernel k){
	ker=k.data;
	kw=k.w;
	kh=k.h;

	hkw=kw/2;
	hkh=kh/2;
	ks=kw*kh;

}
void Convolutioner::setImage(Image in){
	img=in.data;
	imgw=in.w;
	imgh=in.h;
}
void Convolutioner::setKernel(Kernel k){
	ker=k.data;
	kw=k.w;
	kh=k.h;
}
double Convolutioner::convoluteAt(uint i,uint j){
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
void printKer(Kernel ker){
	for(uint i=0;i<ker.w*ker.h;i++){
		printf("%lf,",ker.data[i]);
		if(!((i+1)%ker.w)){
			putchar('\n');
		}
	}
}