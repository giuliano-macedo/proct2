#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "../lodepng/lodepng.h"
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
void kmeans(Image in,Image *out,unsigned char pts[],uint k,uint maxIterations){
    unsigned char grey=256/k;
    unsigned char min,currMin,p,newAxis;
    unsigned char *im=in.data,*d=in->data;
    unsigned char *sums=  (unsigned char *)calloc(k);
    unsigned char *noSums=(unsigned char *)calloc(k);
    uint m,x,y,i,choseni,md;

    for(m=0;m<maxIterations;m++){
        for(y=0;y<h;y++){
            for(x=0;x<w;x++){
                p=im[(y*w)+x];
                min=0;
                for(i=0;i<k;i++){
                    currMin=abs(pts[i]-p);
                    if(currMin<min){
                        min=currMin;
                        choseni=i;
                    }
                }
                sums[choseni]+=p;
                noSums[choseni]++;
                d[(y*w)+x]=grey*choseni;
            }
        }
        for(i=0;i<k;i++){
            newAxis=sums[i]/noSums[i];
            md+=Math.abs(newAxis-pts[i]);
            pts[i]=(int)newAxis;
        }
        if(md<=5){
            return;
        }
    }
}
int main(int argc,char** argv){
    if(argc!=3){
        fprintf(stderr, "[Uso] %s [arquivo.png] [saida.png] \n",argv[0]);
        return -1;
    }
    Image img = getImage(argv[1]);
    Image ans = {img.w,img.h,(unsigned char*)malloc(img.h*img.w)};
    kmeans(img,&ans,{0,43,100},3,10);
    
}