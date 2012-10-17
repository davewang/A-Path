//
//  SSGameWaypoint.h
//  A*Path
//
//  Created by Dave on 12-10-17.
//
//

#ifndef __A_Path__SSGameWaypoint__
#define __A_Path__SSGameWaypoint__

#include "GameWaypoint.h"

enum {
	MOVE_TYPE_RUN = 0,
	MOVE_TYPE_JUMP = 1,
	MOVE_TYPE_FLY = 2
};

class SSGameWaypoint : GameWaypoint {
    int moveType;	//RUN, JUMP, FLY, etc
	bool active;	//Is the waypoint active?
	CCPoint launchVector;	//If we need to JUMP, the launch vector
    SSGameWaypoint(CCPoint p, int type, float s);
    static SSGameWaypoint* createWithPositionWithMoveTypeAndSpeedMod(CCPoint p,int type,float s); 
  
};

 
 
#endif /* defined(__A_Path__SSGameWaypoint__) */
