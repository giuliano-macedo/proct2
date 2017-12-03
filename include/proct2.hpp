#include "ImageLoader.hpp"
#include <getopt.h>
#include <dirent.h>
#include <ctype.h>

struct DatasetFolders{
	char* train;
	char* test;
};
typedef struct DatasetFolders DSFolders;

ImageLoader* processArgs(int argc,char** argv,DSFolders* folders);
void printFromFile(char* filename);