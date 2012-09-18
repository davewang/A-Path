//
//  ProjectileAiming.cpp
//  Box2dWater
//
//  Created by Dave on 12-9-2.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#include "ProjectileAiming.h"

CCLayer*  ProjectileAiming::runRecipe()
{
	//Set game area size
	gameAreaSize = ccp(1000/PTM_RATIO,600/PTM_RATIO);	//Box2d units
    
	//Superclass initialization and message
	GameArea2D::runRecipe();
    message->setString("Use the DPad to dodge the projectiles." );
    
	//Add DPad
    this->addDPad();
	 
    
	//Add some geometry
    this->addRandomPolygons(5);
    
	//Create Actor
    this->addActor();
	 
	//Create enemies
    this->addEnemies();
	
	//Create level boundaries
    this->addLevelBoundaries();
	
	//Initial variables
	cameraZoom = 0.45f;
	fireCount = 0;
	missiles = new CCArray(1);
    
	//Step method
    this->schedule(schedule_selector(ProjectileAiming::step));
	    
    return this;
}

void ProjectileAiming::step(float dt)
{
    GameArea2D::step(dt);
 
	
	//Process DPad
	if(dPad->pressed){
		actor->body->ApplyForce(b2Vec2(dPad->pressedVector.x*25.0f, dPad->pressedVector.y*25.0f), actor->body->GetPosition());
		actor->body->SetAngularVelocity(0.0f);
		CCPoint movementVector = ccp(actor->body->GetLinearVelocity().x, actor->body->GetLinearVelocity().y);
        
		actor->body->SetTransform(actor->body->GetPosition(), -1 * GameHelper::vectorToRadians( movementVector) + PI_CONSTANT/2);
	}else{
        actor->stopRunning();
	}
    
	//Follow the actor with the camera
	CCPoint actorPosition = ccp(actor->body->GetPosition().x*PTM_RATIO, actor->body->GetPosition().y*PTM_RATIO);
	this->centerCameraOnGameCoord(actorPosition);
	//Align enemies
	for(int i=0; i<enemies->count(); i++){
		GameActor *enemy = (GameActor*)enemies->objectAtIndex(i); //[enemies objectAtIndex:i];
		CCPoint directionVector = CCPointMake(actor->body->GetPosition().x - enemy->body->GetPosition().x, actor->body->GetPosition().y - enemy->body->GetPosition().y);
		float radians = GameHelper::vectorToRadians(directionVector);
		enemy->body->SetTransform(enemy->body->GetPosition(), -1 * radians + PI_CONSTANT/2);
	}
	
	//Firing projectiles
	fireCount += dt;
	if(fireCount > 1.0f){
		fireCount = 0;
        this->fireMissiles(); 
	}
}

/* Add DPad */
void ProjectileAiming::addDPad(){
     CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
    cache->addSpriteFramesWithFile( "dpad_buttons.plist");
	dPad = new DPad();//new DPad();
	dPad->setPosition(ccp(75,75));
	dPad->setOpacity(100);
    this->addChild(dPad,0);
  //  dPad->setDisplayFrame( cache->spriteFrameByName("d_pad_normal.png"));
}

/* Add Actor */
void ProjectileAiming::addActor() {
	actor = new GameActor();
	actor->gameArea = this;
	actor->setTag(GO_TAG_ACTOR);
	
	actor->bodyDef->type = b2_dynamicBody;
	actor->bodyDef->position.Set(gameAreaSize.x/2,gameAreaSize.y/2);
	actor->bodyDef->userData = actor;
    
	actor->body = world->CreateBody(actor->bodyDef);
	
	actor->circleShape = new b2CircleShape();
	actor->circleShape->m_radius = 0.5f;
	actor->fixtureDef->shape = actor->circleShape;
	actor->fixtureDef->filter.categoryBits = CB_ACTOR;
	actor->fixtureDef->filter.maskBits = CB_EVERYTHING;
	
	actor->body->CreateFixture(actor->fixtureDef);
	
	actor->body->SetLinearDamping(2.0f);
}

/* Add 3 enemies at random positions */
void  ProjectileAiming::addEnemies()
{
	enemies = new CCArray(1);
    
	for(int i=0; i<3; i++){
		GameActor *enemy = new GameActor();
		enemy->gameArea = this;
		enemy->setTag( GO_TAG_ENEMY);
		
		enemy->bodyDef->type = b2_dynamicBody;
		enemy->bodyDef->position.Set((arc4random()%((int)(gameAreaSize.x-2)))+1.0f,(arc4random()%((int)(gameAreaSize.y-2)))+1.0f);
		enemy->bodyDef->userData = enemy;
        
		enemy->body = world->CreateBody(enemy->bodyDef);
		
		enemy->circleShape = new b2CircleShape();
		enemy->circleShape->m_radius = 0.5f;
		enemy->fixtureDef->shape = enemy->circleShape;
		enemy->fixtureDef->filter.categoryBits = CB_ENEMY;
		enemy->fixtureDef->filter.maskBits = CB_EVERYTHING & ~CB_MISSILE;
		
		enemy->body->CreateFixture(enemy->fixtureDef);
		enemies ->addObject(enemy);
	}
}

/*  Each enemy fires a missile object */
void  ProjectileAiming::fireMissiles() {
	for(int i=0; i<enemies->count(); i++){
		GameActor *enemy = (GameActor*)enemies->objectAtIndex(i);
		//Create missile
		GameMisc *missile = new GameMisc();
		missile->gameArea = this;
		missile->setTag( GO_TAG_MISSILE);
		
		missile->bodyDef->type = b2_dynamicBody;
		missile->bodyDef->position.Set( enemy->body->GetPosition().x, enemy->body->GetPosition().y );
		missile->bodyDef->userData = missile;
		
		missile->body = world->CreateBody(missile->bodyDef);
		
		missile->polygonShape = new b2PolygonShape();
		missile->polygonShape->SetAsBox(0.5f, 0.2f);
		missile->fixtureDef->density = 10.0f;
		missile->fixtureDef->shape = missile->polygonShape;
		missile->fixtureDef->filter.categoryBits = CB_MISSILE;
		missile->fixtureDef->filter.maskBits = CB_EVERYTHING & ~CB_MISSILE & ~CB_ENEMY;
		
		missile->body->CreateFixture(missile->fixtureDef);
		
		//Calculate intercept trajectory
		Vector3D *point = this->interceptSrcAndDstAndProjSpeed(missile, actor, 20.0f);;
		if(point){
			//Align missile
			CCPoint pointToFireAt = CCPointMake(point->x, point->y);
			CCPoint directionVector = CCPointMake(pointToFireAt.x - missile->body->GetPosition().x, pointToFireAt.y - missile->body->GetPosition().y);
            
			float radians = GameHelper::vectorToRadians(directionVector);
			missile->body->SetTransform(missile->body->GetPosition(), -1 * radians + PI_CONSTANT/2);
            
			//Fire missile
			CCPoint normalVector =  GameHelper::radiansToVector(radians);//[GameHelper radiansToVector:radians];
			missile->body->SetLinearVelocity( b2Vec2(normalVector.x*20.0f, normalVector.y*20.0f) );
		}
		missiles->addObject(missile); 
	}
}

/* Find the intercept angle given projectile speed and a moving target */
Vector3D*  ProjectileAiming::interceptSrcAndDstAndProjSpeed( GameObject* src,GameObject* dst,float projSpeed)
{
    float tx = dst->body->GetPosition().x - src->body->GetPosition().x;
    float ty = dst->body->GetPosition().y - src->body->GetPosition().y;
    float tvx = dst->body->GetLinearVelocity().x;
    float tvy = dst->body->GetLinearVelocity().y;
    
    //Get quadratic equation components
    float a = tvx*tvx + tvy*tvy - projSpeed*projSpeed;
    float b = 2 * (tvx * tx + tvy * ty);
    float c = tx*tx + ty*ty;
    //Solve quadratic equation
    Vector3D *ts =GameHelper::quadraticABC(a, b, c );
    
    //Find the smallest positive solution
    Vector3D *solution = NULL;
    if(ts){
        float t0 = ts->x;
        float t1 = ts->y;
        float t = MIN(t0,t1);
        if(t < 0){ t = MAX(t0,t1); }
        if(t > 0){
            float x = dst->body->GetPosition().x + dst->body->GetLinearVelocity().x*t;
            float y = dst->body->GetPosition().y + dst->body->GetLinearVelocity().y*t;
            solution = Vector3D::xyz(x, y, 0);
        }
    }
    return solution;
}

/* Our base collision handling routine */
void ProjectileAiming::handleCollisionWithObjAWithObjB(GameObject* objA, GameObject* objB)
{
	//General collision handling
	if(objA->getTag() == GO_TAG_MISSILE){
        this->markBodyForDestruction(objA);
	}else if(objB->getTag() == GO_TAG_MISSILE){
        this->markBodyForDestruction(objB);
	}
}

/* DPad input */
bool ProjectileAiming::hudBegan(CCSet *touches,CCEvent *event)
{
    bool hudTouched = false;
    dPad->ccTouchesBegan(touches, event);
    if(dPad->pressed || touches->count() == 1){ hudTouched = true; }
	return hudTouched;
}
bool ProjectileAiming::hudMoved(CCSet *touches, CCEvent *event)
{
    bool hudTouched = false;
    dPad->ccTouchesMoved(touches, event);
	if(dPad->pressed || touches->count() == 1){ hudTouched = true; }
	return hudTouched;
    
}
bool ProjectileAiming::hudEnded(CCSet *touches, CCEvent *event)
{
    bool hudTouched = false;
    dPad->ccTouchesEnded(touches, event);
	if(dPad->pressed || touches->count() == 1){ hudTouched = true; }
	return hudTouched;
    
}


void ProjectileAiming::cleanRecipe() {
	//Cleanup missiles
    for (int i=0; i<missiles->count(); i++) {
        GameMisc *m = (GameMisc*) missiles->objectAtIndex(i);
        m->release();
        m = NULL;
    }
    missiles->removeAllObjects();
    GameArea2D::cleanRecipe();
	 
}