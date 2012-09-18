//
//  GameWaypoint.h
//  Box2dWater
//
//  Created by Dave on 12-8-30.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__GameWaypoint__
#define __Box2dWater__GameWaypoint__
#include "ObjectCallback.h"
#include "Base.h"
class GameWaypoint: public CCObject
{
public:
    CCPoint position;	//Where you are going to
    float speedMod;	//The relative speed. 0 is not moving. 1 is going as fast as possible.
    float lastDistance;	//How far you were from the waypoint last iteration.
    ObjectCallback *preCallback;		//Call this when we start moving toward the waypoint
    ObjectCallback *postCallback;		//Call this after we reach the waypoint
    static GameWaypoint* createWithPositionWithSpeedMod(CCPoint p,float s);
    GameWaypoint();
    GameWaypoint(CCPoint p,float s);
    //GameWaypoint*  initWithPositionWithSpeedMod(CCPoint p,float s);
    virtual void processPreCallback();
    virtual void processPostCallback();

};

#endif /* defined(__Box2dWater__GameWaypoint__) */
