#include "proct2.hpp"
int main(uint argc,char** argv){
	ImageCategorizer* ic = new ImageCategorizer();
	// TODO process args	
	DIR *dir;
	struct dirent *ent;
	if((dir = opendir ("path/")) == NULL){//TODO
		fprintf(stderr, "Erro pasta de banco de dados não encontrada\n" );
		return 127;	
	}
	while((ent=readdir(dir))!=NULL){
		ip->add(ent->d_name);
	}
	closedir(dir);
	DSItem* dataset = ip->getDS();
	delete ip;
	//classify, calculate percentage,end
	return 0;
}