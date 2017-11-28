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
    ans=*i;

    ans->d=(unsigned char*)malloc(i->w*i->h);
    if(ans->d==NULL)ilError(0);
    return ans;
}
