#ifndef PROCT2

#define PROCT2
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
//...[TODO]
#endif