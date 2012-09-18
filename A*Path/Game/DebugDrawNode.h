//
//  DebugDrawNode.h
//  Box2dWater
//
//  Created by Dave on 12-8-28.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__DebugDrawNode__
#define __Box2dWater__DebugDrawNode__

#include "Base.h"
#include "GLES-Render.h"
#include <stdlib.h>

class DebugDrawNode :public CCLayer{
public:
    b2World* world;
     static DebugDrawNode* createWithWorld(b2World* worldPtr);
    virtual DebugDrawNode* initWithWorld(b2World* worldPtr);
     void draw();
};

 
#endif /* defined(__Box2dWater__DebugDrawNode__) */
