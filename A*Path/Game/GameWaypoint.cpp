//
//  GameWaypoint.cpp
//  Box2dWater
//
//  Created by Dave on 12-8-30.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#include "GameWaypoint.h"

GameWaypoint* GameWaypoint::createWithPositionWithSpeedMod(CCPoint p,float s)
{
    
    return new GameWaypoint(p,s);
}
GameWaypoint::GameWaypoint()
{
}
GameWaypoint::GameWaypoint(CCPoint p,float s)
{
    this->position = p;
    this->speedMod = s;
    this->lastDistance = 1000000.0f;
    this->preCallback = NULL;
    this->postCallback = NULL;
}
void GameWaypoint::processPreCallback()
{
    // preCallback->obj
   CCCallFunc *func = CCCallFunc::create(preCallback->obj, preCallback->callbackfun);
    func->execute();
   
}

void GameWaypoint::processPostCallback()
{
    CCCallFunc *func =  CCCallFunc::create(postCallback->obj, postCallback->callbackfun);
     func->execute(); 
 
}
