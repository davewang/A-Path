//
//  LineOfSight.h
//  A*Path
//
//  Created by Dave on 12-9-11.
//
//

#ifndef __A_Path__LineOfSight__
#define __A_Path__LineOfSight__


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
#include "DPad.h"
#include "RayCast.h"

class GameArea2D;
class GameObject;
class GameSensor;
class GameMisc;
class GameActor;
class GameWaypoint;
class DrawLayer;

class LineOfSight : public GameArea2D
{
public:
	GameActor *actor;
	DrawLayer *_drawLayer;
	CCArray *enemies;
	DPad *dPad;
    virtual CCLayer* runRecipe();
    virtual void setGameAreaSize();
    virtual void step(float dt);
    virtual void followActorWithEnemies();
    virtual void addDPad();
    virtual void addActor();
    virtual void addEnemies();
    virtual void drawLayer();
    virtual void addDrawLayer();
    virtual bool hudBegan(CCSet *touches ,CCEvent *event);
    virtual bool hudMoved(CCSet *touches ,CCEvent *event);
    virtual bool hudEnded(CCSet *touches ,CCEvent *event);
    
};
  
#endif /* defined(__A_Path__LineOfSight__) */
