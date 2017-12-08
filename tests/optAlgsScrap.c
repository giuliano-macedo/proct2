int ceil_log2(unsigned long long x)
{
  static const unsigned long long t[6] = {
    0xFFFFFFFF00000000ull,
    0x00000000FFFF0000ull,
    0x000000000000FF00ull,
    0x00000000000000F0ull,
    0x000000000000000Cull,
    0x0000000000000002ull
  };

  int y = (((x & (x - 1)) == 0) ? 0 : 1);
  int j = 32;
  int i;

  for (i = 0; i < 6; i++) {
    int k = (((x & t[i]) == 0) ? 0 : j);
    y += k;
    x >>= k;
    j >>= 1;
  }

  return y;
}
//part of convoluteAt
uint px=(((int)i-(int)hkw)<0)?i+hkw:(i-hkw);
  uint py=(((int)j-(int)hkh)<0)?j+hkh:(j-hkh);
  for(y=0;y<kh;y++){
    if(py>imgh-1){
      py-=kh;
    }
    for(x=0;x<kw;x++){
      if(px>imgw-1){
        px-=kw;
      }
        ans+=(int)(ker[(y*kw)+x]*img[(py*imgw)+px]);
      px++;
    }
    py++;
  }
void ImageLoader::reset(){
  // ds.clear();
  // dsIndex=0;
  ds.clear();
  uint sz=imgs.size();
  for(uint i=0;i<sz;i++){
    free(imgs[i]);
  }
  imgs.clear();
}
void ImageLoader::carregaImage(const char* filename){
  unsigned char* ans;
  uint w,h;
  lodepng_decode_file(&ans, &w, &h, filename,LCT_GREY,8);
  imgs.push_back(ans);
}
void ImageLoader::run(){
  uint imgi;
  uint sz=imgs.size();
  size_t noParams=params.size();
  for(imgi=0;imgi<sz;imgi++){
    ds.push_back({std::vector<double>(),0});//TODO set Label
    currentData=&(ds[imgi].data);
    mainImg.data=imgs[imgi];

    if(isShapeSet){
      sobel->run(mainImg,&edgeImg);
    }
    for(uint i=0;i<noParams;i++){
      switch(params[i]){
        case ILFRACTDIM_SHA:
          fractdim_shape(&edgeImg);
          break;
        case ILHUMOMENTS_SHA:
          humoments_shape(&edgeImg);
          break;
        case ILFOURIER_TEX:
          fourier_texture(&mainImg);
          break;
        case ILNETACTIVITY_TEX:
          netactivity_texture(&mainImg);
          break;
      }
    }
    printf("\r%.2lf%%",((double)(imgi*100)/sz));
    fflush(stdout);
  }
  putchar('\n');
}