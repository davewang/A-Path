//
//  ProjectileAiming.h
//  Box2dWater
//
//  Created by Dave on 12-9-2.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__ProjectileAiming__
#define __Box2dWater__ProjectileAiming__

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


enum {
	GO_TAG_ENEMY = 2,
	GO_TAG_MISSILE = 3
};

enum {
	CB_EVERYTHING = 0xFFFF,
	CB_WALL = 0x0001,
	CB_ACTOR = 0x0002,
	CB_ENEMY = 0x0004,
	CB_MISSILE = 0x0008
	//0x0010
	//0x0020
	//0x0040
	//0x0080
};


class ProjectileAiming : public GameArea2D
{
public:
    GameActor *actor;
	DrawLayer *drawLayer;
	CCArray *enemies;
	CCArray *missiles;
	DPad *dPad;
	float fireCount;

    virtual CCLayer* runRecipe();
    virtual void step(float dt);
    virtual void addDPad();
    virtual void addActor();
    virtual void addEnemies();
    virtual void fireMissiles();
    virtual Vector3D* interceptSrcAndDstAndProjSpeed( GameObject* src,GameObject* dst,float projSpeed);
    virtual bool hudBegan(CCSet *touches ,CCEvent *event);
    virtual bool hudMoved(CCSet *touches ,CCEvent *event);
    virtual bool hudEnded(CCSet *touches ,CCEvent *event);
    virtual void handleCollisionWithObjAWithObjB(GameObject* objA, GameObject* objB);
//    virtual void handleCollisionWithSensorWithMisc(GameSensor* sensor, GameMisc* misc);
//    virtual void handleCollisionWithMiscWithMisc(GameMisc* a,GameMisc* b);
     virtual void cleanRecipe();

};
#endif /* defined(__Box2dWater__ProjectileAiming__) */
