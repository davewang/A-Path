//
//  DrawingSprites.h
//  Box2dWater
//
//  Created by Dave on 12-8-24.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__DrawingSprites__
#define __Box2dWater__DrawingSprites__


#include "Base.h"
#include "Waypoints.h"
#include "ProjectileAiming.h"
#include "AIFlocking.h"
#include "GridPathfinding.h"
#include "Box2DPathfinding.h"
#include "TileMapPathfinding.h"
class DrawingSprites : public CCLayer
{
public:
    //Waypoints *recipe;
    //AIFlocking *recipe;
   //  GridPathfinding *recipe;
    //Box2DPathfinding *recipe;
    TileMapPathfinding *recipe; 
public:
    virtual bool init();
    virtual void onEnter();  
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    virtual void menuCloseCallback(CCObject* pSender);
    virtual void addMessage();

};
#endif /* defined(__Box2dWater__DrawingSprites__) */
