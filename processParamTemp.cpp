#include <getopt.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <vector>
typedef unsigned int uint;
enum ThreshCode{
	ICSIMPLE_THR,
	ICKMEAN_THR
};
enum IcParamCode{
    START,
	ICFOURIER_SHA,
	ICFRACTDIM_SHA,
	ICHUMOMENTS_SHA,

	ICFOURIER_TEX,
	ICFRACTDIM_TEX,
	ICLBP_TEX,

	iC_NO_ARGTYPES
};
std::vector<int> parseParams(char* in){
    std::vector<int> ans;
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
int main(int argc, char ** argv){
   int             c;
   struct option   long_opt[] ={
      {"help",            no_argument,       NULL, 'h'},
      {"ajuda",           no_argument,       NULL, 'h'},
      {"fourierForma",    required_argument, NULL,ICFOURIER_SHA},
      {"fractalForma",    required_argument, NULL,ICFRACTDIM_SHA},
      {"momemntoshuForma",no_argument      , NULL,ICFRACTDIM_SHA},
      {"fourierTextura",  required_argument, NULL,ICFOURIER_TEX},
      {"fractalTextura",  required_argument, NULL,ICFRACTDIM_TEX},
      {"lbpTextura",      required_argument, NULL,ICLBP_TEX},
      {"simplesLimiar",   required_argument, NULL,ICSIMPLE_THR},//TODO
      {"kmeanLimiar",     required_argument, NULL,ICKMEAN_THR},//TODO
      {"treino",          required_argument, NULL,'t'},//TODO
      {"teste",           required_argument, NULL,'d'},//TODO
      {"dados",           required_argument, NULL,'d'},//TODO
      {NULL,            0,                 NULL, 0  }
   };
	std::vector<int> params;
   while((c = getopt_long(argc, argv, "h", long_opt, NULL)) != -1){
      switch(c){
        case -1:
        case 0:
            break;
        case ICFOURIER_SHA:
			printf("voce digitou %s\n",optarg);
			params = parseParams(optarg);
			for(uint i=0;i<params.size();i++){
				printf("%i\n",params[i]);
			}
			break;
		case ICFRACTDIM_SHA:
			printf("voce digitou %s\n",optarg);
			break;
		case ICHUMOMENTS_SHA:
			printf("voce digitou %s\n",optarg);
			break;
		case ICFOURIER_TEX:
			printf("voce digitou %s\n",optarg);
			break;
		case ICFRACTDIM_TEX:
			printf("voce digitou %s\n",optarg);
			break;
		case ICLBP_TEX:
			printf("voce digitou %s\n",optarg);
			break;
        case 'h':
            // printFromFile("HELP");
            printf("AJUDAPORRA\n");
            return 0;

         case ':':
         case '?':
             fprintf(stderr, "Use `%s --help' para mais informações.\n", argv[0]);
             return -2;

         default:
             fprintf(stderr, "%s: opção invalida -- %c\n", argv[0], c);
             fprintf(stderr, "Use '%s --help' ou '%s --ajuda' para mais informações.\n", argv[0],argv[0]);
             return -2;
        }
    }
    return 0;
}
