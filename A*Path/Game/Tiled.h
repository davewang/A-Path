//
//  Tiled.h
//  A*Path
//
//  Created by Dave on 12-10-14.
//
//

#ifndef __A_Path__Tiled__
#define __A_Path__Tiled__

#include <iostream>
#include "Recipe.h"
#include "GameArea2D.h"
#include "GameActor.h"

#include "DrawLayer.h"
#include "DPad.h"

class Tiled: public GameArea2D
{
public:
    GameActor *actor;
	DrawLayer *_drawLayer;
	DPad *dPad;
	Vector3D *spawnPoint;
    CCTMXTiledMap *tileMap;
    
    
    virtual CCLayer* runRecipe();
    virtual void setGameAreaSize();
    virtual void step(float dt);
    virtual void addDPad();
    virtual void addActor();
    virtual void cameraFollowActor();
    
    virtual bool hudBegan(CCSet *touches ,CCEvent *event);
    virtual bool hudMoved(CCSet *touches ,CCEvent *event);
    virtual bool hudEnded(CCSet *touches ,CCEvent *event);
    virtual void cleanRecipe();
};
#endif /* defined(__A_Path__Tiled__) */
