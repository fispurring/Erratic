#include "CCXWLoader.h"

using namespace cocos2d;

char *CCXWLoader::s_path=NULL;
void CCXWLoader::setPath(char *path)
{
	CCXWLoader::s_path=path;
}

CCImage::EImageFormat CCXWLoader::getImageType(const char *fileName)
{
	int count=strlen(fileName);
	char stuffix4[5]={'\0'};
	char stuffix5[6]={'\0'};
	
	if(count>4)
	{
		strncpy(stuffix4,fileName+count-4,4);
		if(strcmpi(stuffix4,".jpg")==0)
		{
			return CCImage::kFmtJpg;
		}
		else if(strcmpi(stuffix4,".png")==0)
		{
			return CCImage::kFmtPng;
		}
	}
	
	if(count>5)
	{
		strncpy(stuffix5,fileName+count-5,5);
		if(strcmpi(stuffix5,".jpeg")==0)
		{
			return CCImage::kFmtJpg;
		}
	}
	return CCImage::kFmtUnKnown;
}

CCImage *CCXWLoader::createImage(const char *path,const char *fileName)
{
	CCImage *image=NULL;
	FILE *fp=fopen(path,"rb");
	int32_t count=0;
	CCXWFileInfo *fileInfo;
	void *fileData;
	CCImage::EImageFormat imageFmt=CCImage::kFmtUnKnown;

	if(!fp)
	{
		return NULL;
	}

	fseek(fp,-(int)sizeof(int32_t),SEEK_END);
	fread(&count,sizeof(int32_t),1,fp);
	fileInfo=(CCXWFileInfo*)calloc(count,sizeof(CCXWFileInfo));
	fseek(fp,-(int)sizeof(int32_t)-sizeof(CCXWFileInfo)*count,SEEK_END);
	fread(fileInfo,sizeof(CCXWFileInfo),count,fp);
	for(int i=0;i<count;i++)
	{
		if(strcmpi(fileInfo[i].fileName,fileName)==0)
		{
			imageFmt=CCXWLoader::getImageType(fileName);
			if(imageFmt==CCImage::kFmtUnKnown)
			{
				continue;
			}
			image=new CCImage();
			fileData=calloc(fileInfo[i].size,1);
			fseek(fp,fileInfo[i].seek,SEEK_SET);
			fread(fileData,1,fileInfo[i].size,fp);
			image->initWithImageData(fileData,fileInfo[i].size,imageFmt);
			free(fileData);
			break;
		}
	}
	fclose(fp);
	free(fileInfo);
	return image;
}

CCTexture2D *CCXWLoader::createTexture(const char *path,const char *fileName)
{
	CCTexture2D *texture=new CCTexture2D();
	texture->initWithImage(CCXWLoader::createImage(path,fileName));
	return texture;
}

CCSprite *CCXWLoader::create(const char *path,const char *fileName)
{
	CCSprite *sprite=CCSprite::createWithTexture(createTexture(path,fileName));
	return sprite;
}

CCSprite *CCXWLoader::create(const char *fileName)
{
	if(s_path&&strlen(s_path))
	{
		return create(s_path,fileName);
	}
	return NULL;
}