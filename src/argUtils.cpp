#include "proct2.hpp"
const char* PROCT2HELP = "\
[USO] ./proct2 -t [caminho para imagens de treino] -d [caminho para imagens de teste] [opções]\n\
opções disponíveis\n\
";
std::vector<double> parseParams(char* in){
    if(!in)return std::vector<double>();
    std::vector<double> ans;
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
                ans.push_back(atof(&str[0]));
                str.clear();
        }
        else{
            if(isdigit(c)||c=='-'||c=='.'){
                str.push_back(c);
            }
            else{
                return std::vector<double>();
            }
        }
    }
    str.push_back(0);
    ans.push_back(atof(&str[0]));
    return ans;
}
ImageLoader* processArgs(int argc, char ** argv,DSFolders* folders){
   ImageLoader* ans=new ImageLoader();
   int c;
   struct option   long_opt[] ={
      {"help",            no_argument,       NULL, 'h'},
      {"ajuda",           no_argument,       NULL, 'h'},
      {"fractalForma",    required_argument, NULL,ILFRACTDIM_SHA},
      {"momentoshuForma",no_argument      , NULL,ILHUMOMENTS_SHA},
      {"fourierTextura",  required_argument, NULL,ILFOURIER_TEX},
      {"atividaderedeTextura",  required_argument, NULL,ILNETACTIVITY_TEX},
      {"opcoesgauss",     required_argument, NULL,'g'},
      {"treino",          required_argument, NULL,'t'},
      {"teste",           required_argument, NULL,'d'},
      {"dados",           required_argument, NULL,'d'},
      {NULL,            0,                 NULL, 0  }
   };
    std::vector<double> params;
    while((c = getopt_long(argc, argv, "ht:d:g:", long_opt, NULL)) != -1){
        //TODO; check params size for each function, dont make paramparse on certain functions
        params=parseParams(optarg);
        switch(c){
            case -1:
            case 0:
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
            case ILNETACTIVITY_TEX:
                ans->addParam(ILNETACTIVITY_TEX,params);
                break;
            case 'g':
                if(params.size()!=2){
                    fprintf(stderr,"Numero de argumentos invalido\n");
                    exit(-1);
                }
                ans->setSobelOptions(params[0],params[1]);
                break;
            case 'h':
                printf("%s",PROCT2HELP);
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
    if(folders->train==NULL){
        fprintf(stderr,"Erro, porfavor indique uma pasta com imagens de treino com %s -t [pasta]\n",argv[0]);
        exit(127);
    }
    if(folders->test==NULL){
        fprintf(stderr,"Erro, porfavor indique uma pasta com imagens de teste com %s -d [pasta]\n",argv[0]);
        exit(127);
    }
    return ans;
}
