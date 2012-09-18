//
//  DebugDrawNode.cpp
//  Box2dWater
//
//  Created by Dave on 12-8-28.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#include "DebugDrawNode.h"
DebugDrawNode* DebugDrawNode::createWithWorld(b2World *worldPtr)
{
    DebugDrawNode *tem = new DebugDrawNode();
    
    return tem->initWithWorld(worldPtr);
}
DebugDrawNode* DebugDrawNode::initWithWorld(b2World *worldPtr)
{
    
    this->world = worldPtr;
    return this;
    
}
void DebugDrawNode::draw()
{
//    glDisable(GL_TEXTURE_2D);
//	glDisableClientState(GL_COLOR_ARRAY);
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//	
//	world->DrawDebugData();
//	
//	glEnable(GL_TEXTURE_2D);
//	glEnableClientState(GL_COLOR_ARRAY);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    CCLayer::draw();
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    kmGLPushMatrix();
    
    world->DrawDebugData();
    
    kmGLPopMatrix();
    
}