#include "ImageLoader.hpp"
#define hypot(x,y) sqrt(SQR(x)+SQR(y))
void imageToComp(Image im,Comp* ci){
    uint s=im.w*im.h;
    unsigned char* d = im.data;
    uint i;
    Comp c={0,0};
    for(i=0;i<s;i++){
        c.a=(double)d[i];
        ci[i]=c;
    }
}
std::vector<double> sumComplexMagsLayers(Comp* ci,uint w,uint h,uint r,uint maxDistance){
    //soma camadas do circulo da imagem de magnitude
    //como essa implementação do fft faz uma imagem na qual
    //as 4 partições estão rotacionadas 180 graus, portanto o calculo se baseia na
    //distância das bordas
    Comp c;
    std::vector<double> ans;

    uint hw=w/2;
    uint hh=h/2;


    double m;
    uint y,x;
    double dis;
    double temp1,temp2;
    for(uint i=0;i<maxDistance;i++){
        ans.push_back(0);//TODO:Too much zeroes :0
    }
    for(y=0;y<h;y++){
        for(x=0;x<w;x++){
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
            ans[(int)(dis/r)]+=m;

        }
    }
    return ans;
}

void ImageLoader::fourier_texture(Image* oi){
    scaleImageLIN(*oi,&scaledImg,imgDeltax,imgDeltay);
    imageToComp(scaledImg,compImgIn);
    fft(compImgIn,compImgOut,1,scaledImg.w*scaledImg.h,0);
    std::vector<double> sum = sumComplexMagsLayers(compImgOut,scaledImg.w,scaledImg.h,fourier_r,fourier_maxDistanceToCenter);
    currentData->insert( currentData->end(), sum.begin(), sum.end() );//concat vectors
}
