#include "proct2.hpp"
void addSlash(char* str){
	size_t sz = strlen(str);
	if(str[sz-1]!='/'){
		str[sz]='/';
		str[sz+1]=0;
	}
}
size_t countFilesInFolder(char *folder){
	size_t ans = 0;
	DIR * dirp;
	struct dirent * entry;

	dirp = opendir(folder);
	while((entry = readdir(dirp)) != NULL) {
		if(entry->d_type == DT_REG){
		     ans++;
		 }
	}
	closedir(dirp);
	return ans;
}
int main(int argc,char** argv){
	uint i;
	DSFolders dsf={NULL,NULL};
	ImageLoader* il = processArgs(argc,argv,&dsf);
	DIR *dir1,*dir2;
	struct dirent *ent;
	if((dir1 = opendir (dsf.train)) == NULL){
		fprintf(stderr, "Erro pasta de imagens de treino não encontrada\n" );
		return 127;
	}
	if((dir2 = opendir (dsf.test)) == NULL){
		fprintf(stderr, "Erro pasta de imagens de teste não encontrada\n" );
		return 127;
	}
	size_t noTrain = countFilesInFolder(dsf.train);
	size_t noTest  = countFilesInFolder(dsf.test );
	printf("Número de arquivos treino:%lu\n",noTrain);
	printf("Número de arquivos teste:%lu\n",noTest);
	il->ds.reserve(noTrain);
	addSlash(dsf.train);
	addSlash(dsf.test);


	char* temp=(char*)malloc(256);
	strcpy(temp,dsf.train);
	readdir(dir1);//.
	readdir(dir1);//..
	ent=readdir(dir1);
	size_t sz=strlen(temp);
	temp=strcat(temp,ent->d_name);

	il->init(temp);

	printf("Gerando treino.arff\n");
	i=1;
	while((ent=readdir(dir1))!=NULL){
		i++;
		temp[sz]=0;
		temp=strcat(temp,ent->d_name);
		il->addImage(temp);
		printf("\r%.2lf%%",((double)(i*100)/noTrain));
		fflush(stdout);
	}
	putchar('\n');
	closedir(dir1);

	il->saveArff("treino.arff");
	il->reset();
	il->ds.reserve(noTest);
	strcpy(temp,dsf.test);

	readdir(dir2);//.
	readdir(dir2);//..
	sz=strlen(temp);
	i=0;
	printf("Gerando teste.arff\n");
	while((ent=readdir(dir2))!=NULL){
		i++;
		temp[sz]=0;
		temp=strcat(temp,ent->d_name);
		il->addImage(temp);
		printf("\r%.2lf%%",((double)(i*100)/noTest));
		fflush(stdout);
	}

	il->saveArff("teste.arff");
	closedir(dir2);
	putchar('\n');


	//classify, calculate percentage,end
	return 0;
}
