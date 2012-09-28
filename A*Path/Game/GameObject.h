//
//  GameObject.h
//  Box2dWater
//
//  Created by Dave on 12-8-28.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__GameObject__
#define __Box2dWater__GameObject__
#include "Base.h"
#include "GameObjectTypes.h"

//#import "b2Body.h"
 
//#import "b2Fixture.h"
class GameArea2D;
class GameObject : public CCNode{
public:
    GameArea2D *gameArea;
    b2Body *body;
    b2BodyDef *bodyDef;
    b2FixtureDef *fixtureDef;
    b2PolygonShape *polygonShape;
    b2CircleShape *circleShape;
    CCSprite *sprite;
    int typeTag;
    bool markedForDestruction;
    GameObject();
    void initBox2D();
    virtual int type();
    virtual void release();
    
};

#endif /* defined(__Box2dWater__GameObject__) */
