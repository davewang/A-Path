//
//  Box2DPathfinding.h
//  A*Path
//
//  Created by Dave on 12-10-13.
//
//

#ifndef __A_Path__Box2DPathfinding__
#define __A_Path__Box2DPathfinding__

#include <iostream>
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
#include "AStarNode.h"
#include "AStarPathNode.h"
#include "RayCast.h"

class GameArea2D;
class GameObject;
class GameSensor;
class GameMisc;
class GameActor;
class GameWaypoint;
class DrawLayer;

//Interface
class Box2DPathfinding : public GameArea2D
{
public:
	GameActor *actor;
	float actorRadius;
	DrawLayer *_drawLayer;
	
	CCArray *grid;
	float nodeSpace;	//The space between each node, increase this to increase A* efficiency at the cost of accuracy
	int gridSizeX;
	int gridSizeY;
    
    CCLayer* runRecipe();
    
    virtual void addNeighborToGridNodeXY(AStarNode* node,int x,int y);
    virtual void step(float dt);
    virtual void addActor();
    virtual void drawLayer();
    virtual void addDrawLayer();
//    virtual void handleCollisionWithObjAWithObjB(GameObject* objA,GameObject* objB);
//   
//    virtual void handleCollisionWithSensorWithMisc(GameSensor* sensor, GameMisc* misc);
//    virtual void handleCollisionWithMiscWithMisc(GameMisc* a,GameMisc* b);
    virtual void tapWithPoint(CCPoint point);

};

#endif /* defined(__A_Path__Box2DPathfinding__) */
