//
//  GameActor.h
//  Box2dWater
//
//  Created by Dave on 12-8-30.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__GameActor__
#define __Box2dWater__GameActor__

#import "GameObject.h"
#define WAYPOINT_DIST_THRESHOLD 16.0f
#define TIMES_BLOCKED_FAIL 2
class GameWaypoint;
class GameActor:public GameObject{
public:
    CCArray *waypoints;
    int timesBlocked;
    float32 lastAngularVelocity;
    CCPoint lastVelocity;
    string color;
    
    float runSpeed;
    int direction;
    float animCount;
    
    GameActor();
    
    void addWaypoint(GameWaypoint* waypoint);
    void processWaypoints();
    void runWithVectorWithSpeedModWithConstrain(CCPoint v,float speedMod   ,bool constrain);
    void stopRunning();
    static CCPoint  getNormalVectorFromDirection(int dir);
    void animateActor();
    void timeShiftVectMagnitude(float seconds,CCPoint vect,float magnitude);
};
#endif /* defined(__Box2dWater__GameActor__) */
