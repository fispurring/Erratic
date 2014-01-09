#ifndef __CCXWLOADER_H__
#define __CCXWLOADER_H__

#include <stdint.h>
#include <Windows.h>
#include "cocos2d.h"

using namespace cocos2d;

typedef struct {
	int32_t size;
	int32_t seek;
	char fileName[MAX_PATH];
}CCXWFileInfo;

class CCXWLoader {
private:
	static char *s_path;
	static CCImage::EImageFormat getImageType(const char *fileName);
public:
	static void setPath(char *path);
	static CCImage *createImage(const char *path,const char *fileName);
	static CCTexture2D *createTexture(const char *path,const char *fileName);
	static CCSprite *create(const char *path,const char *fileName);
	static CCSprite *create(const char *path);
};
#endif