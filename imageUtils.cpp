static Image ImageLoader::getImage(const char* filename){
	Image ans;

	uint error = lodepng_decode_file(&ans.data, &ans.w, &ans.h, filename,LCT_GREY,8);
	if(error){
		fprintf(stderr,"LODE: ERRO %u %s\n", error, lodepng_error_text(error));
		fprintf(stderr, "Pulando arquivo:%s\n",filename );
		ans.data=NULL;
	}

  return ans;
}
static Image newImageFrom(Image* i){
    Image ans;
	ans.h=i->h;
    ans.w=i->w;

    ans.d=(unsigned char*)malloc(i->w*i->h);
    if(!ans->d)ilError(0);
    return ans;
}
static Image newImage(uint h,uint w){
	Image ans={NULL,480,640};
    ans.data=(unsigned char*)malloc(i.w*i.h);
    if(!ans.data)ilError(0);
	return ans;
}
static void saveImage(Image *i,char* filename){
	uint s=lodepng_encode_file(filename,i->data, i->w, i->h,LCT_GREY, 8));
	if(!s){
		fprintf(stderr, "Falha ao salvar imagem %u\n",s);
		exit(127);
	}
}
