#include "ImageLoader.hpp"
#include <getopt.h>
#include <ctype.h>

struct DatasetFolders{
	char* train;
	char* test;
};
typedef struct DatasetFolders DSFolders;

ImageLoader* processArgs(int argc,char** argc,DSFolders* folders);
void printFromFile(char* filename);