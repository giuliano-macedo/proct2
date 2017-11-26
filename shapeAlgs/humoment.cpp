#include "../proct2.hpp"
double regularMomentOrder(Image* i,double p,double q){
	double ans=0;
	uint p;
	unsigned char* im=i->data;
	uint w=i->w;
	uint h=i->h;
	for(uint y=0;y<h;y++){   
	    for(uint x=0;x<w;x++){
            p=im[(y*w)+x];
            if(p)ans+=pow(x,p)*pow(y,q);
	    }
	}
	return ans;
}
double mu(Image* i,double a,double xmean,double ymean,double p,double q){
	double ans=0;
	uint p;
	unsigned char* im=i->data;
	uint w=i->w;
	uint h=i->h;
	for(uint y=0;y<h;y++){   
	    for(uint x=0;x<w;x++){
            p=im[(y*w)+x];
            if(p)ans+=pow((x-xmean),p)*pow((y-ymean),q);
	    }
	}
	double gamma=((p+q)/2)+1
	return ans/pow(a,gamma);
}
ImageLoader::humoment_shape(Image* i){
	//TODO REVIEW FORMULA
	double a=regularMomentOrder(i,1,1);
	double xm=regularMomentOrder(i,1,0)/a;
	double ym=regularMomentOrder(i,0,1)/a;

	double u11=mu(i,a,xm,ym,1,1);
	double u20=mu(i,a,xm,ym,2,0);
	double u02=mu(i,a,xm,ym,0,2);
	
	double u21=mu(i,a,xm,ym,2,1);
	double u12=mu(i,a,xm,ym,1,2);

	double u30=mu(i,a,xm,ym,3,0);
	double u03=mu(i,a,xm,ym,0,3);

	double temp1,temp2;
	temp1=u20+u02;

	currentData.push_back(temp1);//phi1

	currentData.push_back((temp1*temp1)+(4*u11*u11));//phi2

	temp1=u30-(3*u12);
	temp2=(3*u21)-u03;
	currentData.push_back((temp1*temp1)+(temp2*temp2))//phi3

	temp1=u30+u12;
	temp2=u21+u03;

	temp1*=temp1;
	temp2*=temp2;
	currentData.push_back(temp1+temp2);//phi4

	currentData.push_back((u30-(3*u12))*(u30+u12)*(temp1-(3*temp2))+((3*u21-u03)*(u21+u03)*((3*temp1)-temp2)));//phi5

	currentData.push_back( ((u20-u02)*(temp1-temp2)) +(4*u11*(u30+u12)*(u21+u03))  );//phi6

	currentData.push_back( (((3*u21)-u03)*(u30+u12)*(temp1-3*temp2)) - ((u30 - (3*u12))*(u21+u03)*((3*temp1) - temp2)));//phi7

}