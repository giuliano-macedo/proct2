//Deixando sem uso, kmeans não é muito bom com imagens em tons de cinza
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "lodepng.h"
#include<time.h>
typedef struct Image{
    unsigned char* data;
    uint w;
    uint h;
} Image;
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
void kmeans(Image in,Image *out,unsigned char* pts,uint k,uint maxIterations){
    unsigned char grey=256/k;
    unsigned char min,currMin,p,newAxis;
    unsigned char *im=in.data,*d=out->data;
    unsigned char *sums=  (unsigned char *)calloc(k,1);
    unsigned char *noSums=(unsigned char *)calloc(k,1);
    uint m,x,y,i,choseni,md;
    uint w=in.w;
    uint h=in.h;

    for(m=0;m<maxIterations;m++){
        for(y=0;y<h;y++){
            for(x=0;x<w;x++){
                p=im[(y*w)+x];
                choseni=0;
                min=255;
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
        md=0;
        for(i=0;i<k;i++){
            // newAxis=(noSums[i])?(sums[i]/noSums[i]):pts[i];
            newAxis=(noSums[i])?(sums[i]/noSums[i]):pts[i];
            md=abs(newAxis-pts[i]);
            if(md<=5){
                printf("Parando moveu mt pouco\n");
                return;
            }
            pts[i]=newAxis;
            
            printf("%i,",noSums[i]);
            
            noSums[i]=0;
            sums[i]=0;
        }
        putchar('\n');
    }
    printf("Parando estorou\n");
}
int main(int argc,char** argv){
    srand(time(0));
    if(argc!=3){
        fprintf(stderr, "[Uso] %s [arquivo.png] [saida.png] \n",argv[0]);
        return -1;
    }
    Image img = getImage(argv[1]);
    Image ans = {(unsigned char*)malloc(img.h*img.w),img.w,img.h};
    unsigned char pts[5];
    for(int i=0;i<5;i++){
        pts[i]=rand();
        printf("%i,",pts[i]);
    }
    printf("\n--------------\n");
    // unsigned char pts[5]={98,193,87,204,60};
    kmeans(img,&ans,pts,5,5);
    saveImage(&ans,argv[2]);

}