//
//  DrawLayer.h
//  Box2dWater
//
//  Created by Dave on 12-8-30.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__DrawLayer__
#define __Box2dWater__DrawLayer__
#include "Recipe.h"
 
 
class DrawLayer : public CCLayer
{
public:
	Recipe *recipe;
    virtual void draw();
};

 
 
#endif /* defined(__Box2dWater__DrawLayer__) */
