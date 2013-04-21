#include <Windows.h>
#include "CCXWFileInfo.h"


CCXWFileInfoNode *ccxw_createNode(char *fileName,int size,int seek)
{
	CCXWFileInfoNode *node=(CCXWFileInfoNode *)calloc(1,sizeof(CCXWFileInfoNode));
	strcpy(node->fileInfo.fileName,fileName);
	node->fileInfo.size=size;
	node->fileInfo.seek=seek;
	node->next=NULL;
	return node;
}

void ccxw_appendNode(CCXWFileInfoNode *node,char *fileName,int size,int seek)
{
	if(node==NULL)
	{
		return;
	}
	if(node->next!=NULL)
	{
		ccxw_appendNode(node->next,fileName,size,seek);
	}
	else
	{
		CCXWFileInfoNode *lastNode=ccxw_createNode(fileName,size,seek);
		node->next=lastNode;
	}
}

void ccxw_freeList(CCXWFileInfoNode *head)
{
	if(head)
	{
		if(head->next)
		{
			ccxw_freeList(head->next);
		}
		free(head);
	}
}
