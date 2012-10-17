//
//  TileMapPathfinding.h
//  A*Path
//
//  Created by Dave on 12-10-15.
//
//

#ifndef __A_Path__TileMapPathfinding__
#define __A_Path__TileMapPathfinding__

#include <iostream>
#include "Tiled.h"
#include "AStarNode.h"
#include "Recipe.h"
#include "RayCast.h"
#include "DrawLayer.h"

#include "AStarPathNode.h"

#include "GameWaypoint.h"
class AStarPathNode;
class RayCastAnyCallback;
class TileMapPathfinding : public Tiled{
public:
    CCArray *grid;
	float actorRadius;
 
virtual CCLayer* runRecipe();
virtual void addNeighborToGridNodeXY(AStarNode* node,int x,int y);
virtual void tapWithPoint(CCPoint point);
virtual bool hudBegan(CCSet *touches ,CCEvent *event);
virtual bool hudMoved(CCSet *touches ,CCEvent *event);
virtual bool hudEnded(CCSet *touches ,CCEvent *event);
virtual void drawLayer();
virtual void addDrawLayer();
virtual void cameraFollowActor();
virtual void step(float dt);
virtual void setCameraZoom(float zoom);

};
#endif /* defined(__A_Path__TileMapPathfinding__) */
