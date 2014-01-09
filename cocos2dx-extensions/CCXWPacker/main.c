#include "stdio.h"
#include "CCXWPacker.h"

int main(int argc,char *argv[])
{
	char *path=argv[1];
	if(argc<=1)
	{
		printf("Please input directory name and try again.\n");
		exit(0);
	}

	ccxw_pack(path);
	return 0;
}