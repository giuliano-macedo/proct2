#include "proct2.hpp"
std::vector<std::string> LABELS;
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
uint processLabel(char* filename){
	std::string* ans=new std::string();
	uint i=0;
	uint sz=LABELS.size();
	char c;
	while((c=filename[i++])){
		if(c=='_')break;
		ans->push_back(c);
	}
	ans->push_back(0);
	for(i=0;i<sz;i++){
		if(*ans==LABELS[i]){
			delete ans;
			return i;

		}
	}
	LABELS.push_back(*ans);
	return sz;
	

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
	printf("No. de arquivos treino:%lu\n",noTrain);
	printf("No. de arquivos teste: %lu\n",noTest);
	il->ds.reserve(noTrain);
	addSlash(dsf.train);
	addSlash(dsf.test);


	char* temp=(char*)malloc(256);
	uint actLabel;

	strcpy(temp,dsf.train);
	readdir(dir1);//.
	readdir(dir1);//..
	ent=readdir(dir1);
	size_t sz=strlen(temp);
	temp=strcat(temp,ent->d_name);


	actLabel=processLabel(ent->d_name);
	il->init(temp,actLabel);
	// il->setLabels(LABELS);
	// il->saveArff("foo.arff");
	// return 0;

	printf("Gerando treino.arff\n");
	i=1;
	while((ent=readdir(dir1))!=NULL){
		i++;
		temp[sz]=0;
		temp=strcat(temp,ent->d_name);
		actLabel=processLabel(ent->d_name);
		il->processImage(temp,actLabel);
		printf("\r%.2lf%%",((double)(i*100)/noTrain));
		fflush(stdout);
	}
	putchar('\n');
	closedir(dir1);
	il->setLabels(LABELS);
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
		actLabel=processLabel(ent->d_name);
		il->processImage(temp,actLabel);
		printf("\r%.2lf%%",((double)(i*100)/noTest));
		fflush(stdout);
	}
	il->saveArff("teste.arff");
	closedir(dir2);
	putchar('\n');


	//classify, calculate percentage,end
	return 0;
}
