//
//  DrawingSprites.h
//  Box2dWater
//
//  Created by Dave on 12-8-24.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__DrawingSprites__
#define __Box2dWater__DrawingSprites__

//#include <iostream>
//#include "cocos2d.h"
#include "Base.h"
#include "Waypoints.h"
#include "ProjectileAiming.h"
#include "AIFlocking.h"
#include "GridPathfinding.h"
class DrawingSprites : public CCLayer
{
public:
    //Waypoints *recipe;
   // AIFlocking *recipe;
     GridPathfinding *recipe;
public:
    virtual bool init();
    virtual void onEnter();  
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    virtual void menuCloseCallback(CCObject* pSender);
    virtual void addMessage();
    // implement the "static node()" method manually
    //LAYER_NODE_FUNC(DrawingSprites);
//private:
  //  virtual cocos2d::CCLayer* runRecipe();
};
#endif /* defined(__Box2dWater__DrawingSprites__) */
