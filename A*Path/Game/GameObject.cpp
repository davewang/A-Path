//
//  GameObject.cpp
//  Box2dWater
//
//  Created by Dave on 12-8-28.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#include "GameObject.h"
#include "GameArea2D.h"

GameObject::GameObject():CCNode()
{
    //CCNode::CCNode();
    this->initBox2D();
    markedForDestruction = false;
    sprite = NULL;
    gameArea = NULL;
//    body = NULL;
//    bodyDef = NULL;
//    fixtureDef = NULL;
//    polygonShape = NULL;
//    circleShape = NULL;
    
}
void GameObject::initBox2D()
{
    bodyDef = new b2BodyDef();
    fixtureDef = new  b2FixtureDef();
 
	//Initial fixture settings
	fixtureDef->density = 1.0f;
	fixtureDef->friction = 0.5f;
	fixtureDef->restitution = 0.3f;
    
	bodyDef->userData = this;
}
void GameObject::release()
{

    delete bodyDef;
    delete fixtureDef;
    delete polygonShape;
    delete circleShape;
    sprite->release();
    gameArea->release();
}
int GameObject::type()
{
    return GO_TYPE_NONE;
}
