//
//  Recipe.h
//  Box2dWater
//
//  Created by Dave on 12-8-24.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__Recipe__
#define __Box2dWater__Recipe__
#include "Base.h"
#include "GameHelper.h"
enum {
	TAG_EFFECT_NODE = 0
};
class Recipe: public CCLayer{
    
public:
    //CCLabelBMFont *message;
    CCLabelTTF *message;
public:


    Recipe();
    virtual CCLayer* runRecipe();
    virtual void cleanRecipe();
    virtual void resetMessage();
    virtual void showMessage(const char *m);
    virtual void appendMessage(const char *m);
    virtual void appendMessageWithWrap(const char *m);
    virtual void drawLayer();

};


#endif /* defined(__Box2dWater__Recipe__) */
