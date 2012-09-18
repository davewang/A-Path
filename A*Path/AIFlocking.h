//
//  AIFlocking.h
//  A*Path
//
//  Created by Dave on 12-9-11.
//
//

#ifndef __A_Path__AIFlocking__
#define __A_Path__AIFlocking__
#include "ProjectileAiming.h"
#include "LineOfSight.h"
class AIFlocking : public LineOfSight
{
public:
    virtual CCLayer* runRecipe();
    virtual void setGameAreaSize();
    virtual void processBoids();
    
    virtual void step(float dt);
    virtual void addActor();
    virtual void addEnemies();
    virtual void followActorWithEnemies();
    
    virtual b2Vec2 boidRule1(GameActor* b);
    virtual b2Vec2 boidRule2(GameActor* b);
    virtual b2Vec2 boidRule3(GameActor* b);

};
#endif /* defined(__A_Path__AIFlocking__) */
