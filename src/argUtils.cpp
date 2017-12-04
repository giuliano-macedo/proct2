#include "proct2.hpp"
void printFromFile(const char* filename){
    char buff[128];
    FILE* f = fopen(filename,"r");
    if(!f)fprintf(stderr,"Erro ao abrir arquivo %s\n",filename);
    size_t s;
     while((s=fread(buff,1,128,f))){
        buff[s]=0;
        printf("%s",buff);
    }
}
std::vector<int> parseParams(char* in){
    std::vector<int> ans;
    if(in==NULL){
        ans.push_back(0);
        return ans;
    }
    std::vector<char> str;
    char c;
    uint i=0;
    while((c=in[i++])){
        if(c==','){
                str.push_back(0);
                ans.push_back(atoi(&str[0]));
                str.clear();
        }
        else{
            if(isdigit(c)||c=='-'){
                str.push_back(c);
            }
            else{
                fprintf(stderr, "Erro ao processar argumentos\n");
                exit(127);
            }
        }
    }
    str.push_back(0);
    ans.push_back(atoi(&str[0]));
    return ans;
}
ImageLoader* processArgs(int argc, char ** argv,DSFolders* folders){
   ImageLoader* ans=new ImageLoader();
   int c;
   struct option   long_opt[] ={
      {"help",            no_argument,       NULL, 'h'},
      {"ajuda",           no_argument,       NULL, 'h'},
      {"fourierForma",    required_argument, NULL,ILFOURIER_SHA},
      {"fractalForma",    required_argument, NULL,ILFRACTDIM_SHA},
      {"momemntoshuForma",no_argument      , NULL,ILFRACTDIM_SHA},
      {"fourierTextura",  required_argument, NULL,ILFOURIER_TEX},
      {"fractalkmeansTextura",  required_argument, NULL,ILFRACTDIMKMEANS_TEX},
      {"simplesLimiar",   required_argument, NULL,ILSIMPLE_THR},
      {"kmeanLimiar",     required_argument, NULL,ILKMEAN_THR},
      {"treino",          required_argument, NULL,'t'},
      {"teste",           required_argument, NULL,'d'},
      {"dados",           required_argument, NULL,'d'},
      {NULL,            0,                 NULL, 0  }
   };
    std::vector<int> params;
    while((c = getopt_long(argc, argv, "ht:d:", long_opt, NULL)) != -1){
        //TODO; check params size for each function, dont make paramparse on certain functions
        params=parseParams(optarg);
        switch(c){
            case -1:
            case 0:
                break;
            case ILFOURIER_SHA:
                ans->addParam(ILFOURIER_SHA,params);
                break;
            case ILFRACTDIM_SHA:
                ans->addParam(ILFRACTDIM_SHA,params);
                break;
            case ILHUMOMENTS_SHA:
                ans->addParam(ILHUMOMENTS_SHA,params);
                break;
            case ILFOURIER_TEX:
                ans->addParam(ILFOURIER_TEX,params);
                break;
            case ILFRACTDIMKMEANS_TEX:
                ans->addParam(ILFRACTDIMKMEANS_TEX,params);
                break;
            case ILSIMPLE_THR:
                ans->setThresh(ILSIMPLE_THR,params);
                break;
            case ILKMEAN_THR:
                ans->setThresh(ILKMEAN_THR,params);
                break;
            case 'h':
                printFromFile("HELP");
                exit(0);
            case 'd':
                folders->test=optarg;
                break;
            case 't':
                folders->train=optarg;
                break;
            case ':':
            case '?':
                fprintf(stderr, "Use `%s --help' para mais informações.\n", argv[0]);
                exit(-2);
            default:
                fprintf(stderr, "%s: opção invalida -- %c\n", argv[0], c);
                fprintf(stderr, "Use '%s --help' ou '%s --ajuda' para mais informações.\n", argv[0],argv[0]);
                exit(-2);
        }
    }
    if(ans->isShapeSet&&!ans->isThreshSet){
        fprintf(stderr, "Erro, se uma função de forma for selecionada, uma de limiarização também precisa ser\n");
        exit(127);
    }
    if(folders->train==NULL){
        fprintf(stderr,"Erro, porfavor indique uma pasta com imagens de treino com %s -t [pasta]\n",argv[0]);
        exit(127);
    }
    if(folders->test==NULL){
        fprintf(stderr,"Erro, porfavor indique uma pasta com imagens de teste com %s -d [pasta]\n",argv[0]);
        exit(127);
    }
    return 0;
}
