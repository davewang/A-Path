//
//  Box2DBodyInfo.h
//  Box2dWater
//
//  Created by Dave on 12-8-30.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__Box2DBodyInfo__
#define __Box2dWater__Box2DBodyInfo__
#include "Base.h"
#include "GameObject.h"
class Box2DBodyInfo: CCObject{
public:
	b2BodyDef* bodyDef;
	b2FixtureDef* fixtureDef;
	GameObject* gameObject;
    static Box2DBodyInfo* create();
    virtual void release();
};

#endif /* defined(__Box2dWater__Box2DBodyInfo__) */
