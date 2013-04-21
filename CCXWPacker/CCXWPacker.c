#include <stdio.h>
#include <Windows.h>
#include "CCXWFileInfo.h"

#define SALT 0x1e

void getOutputFileName(char *outputFileName,const char *path)
{
	char *delim="/\\";
	char *result=NULL;
	strcpy(outputFileName,path);
	result=strtok(outputFileName,delim);
	while(result!=NULL)
	{
		strcpy(outputFileName,result);
		result=strtok(NULL,delim);
	}
	strcat(outputFileName,".ccxrc");
}

void ccxw_writeFileInfo(CCXWFileInfoNode *head,FILE *pRCFile)
{
	CCXWFileInfoNode *node=head;
	int32_t count=0;
	while(node)
	{
		void *fileInfo=calloc(sizeof(CCXWFileInfo),1);
		memcpy(fileInfo,&node->fileInfo,sizeof(CCXWFileInfo));
		fwrite(fileInfo,sizeof(CCXWFileInfo),1,pRCFile);
		free(fileInfo);
		count++;
		node=node->next;
	}
	fwrite(&count,sizeof(int32_t),1,pRCFile);
}

void ccxw_writeSalt(FILE *pRCFile)
{
	void *salt=calloc(1,1);
	*((byte *)salt)=SALT;
	fwrite(salt,1,1,pRCFile);
	free(salt);
}

void ccxw_writeFileData(const char *path,WIN32_FIND_DATA ffd,FILE *pRCFile)
{
	void *fileData;
	char filePath[MAX_PATH];
	FILE *fp;

	printf("Package file :%s\n",ffd.cFileName);
	strcpy(filePath,path);
	strcat(filePath,"/");
	strcat(filePath,ffd.cFileName);

	fp=fopen(filePath,"rb");
	fileData=calloc(ffd.nFileSizeLow,1);
	fread(fileData,1,ffd.nFileSizeLow,fp);
	fwrite(fileData,1,ffd.nFileSizeLow,pRCFile);
	free(fileData);
	fclose(fp);
}

void ccxw_pack(const char *path)
{
	char outputFileName[MAX_PATH];
	char directory[MAX_PATH];
	WIN32_FIND_DATA ffd;
	HANDLE hFind=INVALID_HANDLE_VALUE;
	FILE *pRCFile;
	CCXWFileInfoNode *head=NULL;
	PVOID oldValue;

	Wow64DisableWow64FsRedirection(&oldValue);

	strcpy(directory,path);
	strcat(directory,"\\*");
	hFind=FindFirstFile(directory,&ffd);
	if(hFind==INVALID_HANDLE_VALUE)
	{
		printf("Can't Find Resource files!\n");
		exit(0);
	}

	getOutputFileName(outputFileName,path);
	pRCFile=fopen(outputFileName,"wb+");
	ccxw_writeSalt(pRCFile);
	do {
		if((ffd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0)
		{
			if(head)
			{
				ccxw_appendNode(head,ffd.cFileName,ffd.nFileSizeLow,ftell(pRCFile));
			}
			else
			{
				head=ccxw_createNode(ffd.cFileName,ffd.nFileSizeLow,ftell(pRCFile));
			}
			ccxw_writeFileData(path,ffd,pRCFile);
		}
	} while(FindNextFile(hFind,&ffd));
	ccxw_writeFileInfo(head,pRCFile);
	fclose(pRCFile);
	ccxw_freeList(head);
	Wow64RevertWow64FsRedirection(oldValue);
}

