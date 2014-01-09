Open and build the project CCXWPacker.You'll get the target file CCXWPacker.exe.  
To pack:  

	CCXWPacker.exe <Resources folder>  

You'll get the resources package named "FolderName.ccxrc".

To Load Resources:  

Copy the files,which in CCXWLoader folder,to your cocos2d-x project.  
Example Code:  

	#include "CCXWLoader.h"
	
	//your code
	
	//Load CCSprite from Resources file
	CCSprite *sprite=CCXWLoader::create("PackagePath.ccxrc","ImageName.png");
	//or
	CCXWLoader::setPath("PackagePath.ccxrc");
	CCSprite *sprite=CCXWLoader::create("ImageName1.png");
	CCSprite *sprite=CCXWLoader::create("ImageName2.png");
	CCSprite *sprite=CCXWLoader::create("ImageName3.png");
	
	//your code

