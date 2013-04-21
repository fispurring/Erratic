#ifndef __CCXWFILE_INFO_H__
#define __CCXWFILE_INFO_H__

#include <stdint.h>

typedef struct {
	int32_t size;
	int32_t seek;
	char fileName[MAX_PATH];
}CCXWFileInfo;

typedef struct node{
	CCXWFileInfo fileInfo;
	struct node *next;
}CCXWFileInfoNode;

CCXWFileInfoNode *ccxw_createNode(char *fileName,int size,int seek);

void ccxw_appendNode(CCXWFileInfoNode *node,char *fileName,int size,int seek);

void ccxw_freeList(CCXWFileInfoNode *head);
#endif