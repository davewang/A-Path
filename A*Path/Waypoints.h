//
//  Waypoints.h
//  Box2dWater
//
//  Created by Dave on 12-8-30.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__Waypoints__
#define __Box2dWater__Waypoints__
 
#include "Recipe.h"
#include "GameArea2D.h"
#include "GameObject.h"
#include "GameSensor.h"
#include "GameMisc.h"
#include "GameHelper.h"
#include "Vector3D.h"
#include "GameActor.h"
#include "GameWaypoint.h"
#include "DrawLayer.h"
class GameArea2D;
class GameObject;
class GameSensor;
class GameMisc;
class GameActor;
class GameWaypoint;
class DrawLayer;

class Waypoints : public GameArea2D
{
public:
	GameActor *actor;
	DrawLayer *_drawLayer;
    virtual CCLayer* runRecipe();
    virtual void step(float dt);
    virtual void addActor();
    virtual void drawLayer();
    virtual void addDrawLayer();
    virtual void tapWithPoint(CCPoint point);
    virtual void reachedWaypoint();
    virtual void movingToWaypoint();
};
 







 
#endif /* defined(__Box2dWater__Waypoints__) */
