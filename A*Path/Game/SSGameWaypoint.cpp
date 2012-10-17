//
//  SSGameWaypoint.cpp
//  A*Path
//
//  Created by Dave on 12-10-17.
//
//

#include "SSGameWaypoint.h"
SSGameWaypoint*  SSGameWaypoint::createWithPositionWithMoveTypeAndSpeedMod(CCPoint p, int type, float s)
{
    
    SSGameWaypoint *ss = new SSGameWaypoint(p,type,s);
    ss->autorelease();
    return ss;
}
SSGameWaypoint::SSGameWaypoint(CCPoint p, int type, float s):GameWaypoint()
{
    position = p;
    speedMod = s;
    lastDistance = 1000000.0f;
    moveType = type;
    active = true;
    postCallback = NULL;
    preCallback = NULL;
}
 
 
