#include "proct2.hpp"
int main(int argc,char** argv){
	DSFolders dsf={NULL,NULL};
	ImageLoader* il = processArgs(argc,argv,&dsf);
	// DIR *dir;
	// struct dirent *ent;
	// if((dir = opendir (dsf.train)) == NULL){
	// 	fprintf(stderr, "Erro pasta de banco de dados não encontrada\n" );
	// 	return 127;	
	// }
	// ent=readdir(dir);
	// il->init(ent->d_name);
	// while((ent=readdir(dir))!=NULL){
	// 	il->addImage(ent->d_name);
	// }
	// closedir(dir);
	// std::vector<DSItem> dataset = il->getDS();
	// delete il;
	//classify, calculate percentage,end
	return 0;
}