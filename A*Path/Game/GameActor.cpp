//
//  GameActor.cpp
//  Box2dWater
//
//  Created by Dave on 12-8-30.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#include "GameActor.h"
#include "GameWaypoint.h"
#include "GameHelper.h"
#include "GameArea2D.h"
GameActor::GameActor():GameObject()
{
        //GameObject::GameObject();
        waypoints = new CCArray(2);
		runSpeed = 10.0f;
		timesBlocked = 0;
		lastAngularVelocity = 0.0f;
		animCount = 0.0f;
		lastVelocity = ccp(0,0);
		direction = DOWN_LEFT;
		color = "lightblue";
}

void GameActor::addWaypoint(GameWaypoint* waypoint)
{
 
    waypoints->addObject(waypoint);
}

void GameActor::processWaypoints()
{
	bool removeFirstWaypoint = false;
	 
	//The actor's position onscreen
	CCPoint worldPosition = CCPointMake(this->body->GetPosition().x * PTM_RATIO, this->body->GetPosition().y * PTM_RATIO);
	
	//Process waypoints
    
    
    for (int i=0; i<waypoints->count(); i++) {
        GameWaypoint *wp =(GameWaypoint*) waypoints->objectAtIndex(i);
        float distanceToNextPoint = GameHelper::distanceP1toP2(worldPosition, CCPointMake(wp->position.x, wp->position.y)) ;
       
        //If we didn't make progress to the next point, increment timesBlocked
		if(distanceToNextPoint >= wp->lastDistance){
			timesBlocked++;
			
			//Drop this waypoint if we failed to move a number of times
			if(timesBlocked > TIMES_BLOCKED_FAIL){
				distanceToNextPoint = 0.0f;
			}
		}else{
			//If we are just starting toward this point we run our pre-callback
			wp->lastDistance = distanceToNextPoint;
            wp->processPreCallback();
			 
		}
        
		//If we are close enough to the waypoint we move onto the next one
		if(distanceToNextPoint <= WAYPOINT_DIST_THRESHOLD){
			removeFirstWaypoint = true;
			this->stopRunning();
			
			//Run post callback
            wp->processPostCallback();
		 
		}else{
			//Keep running toward the waypoint
			float speedMod = wp->speedMod;
			
			//Slow down close to the waypoint
			if(distanceToNextPoint < this->runSpeed/PTM_RATIO){
				speedMod = (distanceToNextPoint)/(this->runSpeed/PTM_RATIO);
			}
            this->runWithVectorWithSpeedModWithConstrain(ccp(wp->position.x - worldPosition.x, wp->position.y - worldPosition.y), speedMod, false);
			 
			break;
		}
	}
	if(removeFirstWaypoint){
		 
        waypoints->removeObjectAtIndex(0);
		timesBlocked = 0;
	}
}

void GameActor::runWithVectorWithSpeedModWithConstrain(CCPoint v, float speedMod, bool constrain) {
	//Change animation depending on angle
     
	float radians =  GameHelper::vectorToRadians(v);
	float degrees =  GameHelper::radiansToDegrees(radians); 
	CCPoint constrainedVector;	//Vector constained to only the 8 angles
	CCPoint unconstrainedVector = GameHelper::radiansToVector(radians);//[GameHelper radiansToVector:radians];	//Unconstrained vector
    
	degrees += 90.0f;
    
	if(degrees >= 337.5f || degrees < 22.5f){
		direction = LEFT;
	}else if(degrees >= 22.5f && degrees < 67.5f){
		direction = UP_LEFT;
	}else if(degrees >= 67.5f && degrees < 112.5f){
		direction = UP;
	}else if(degrees >= 112.5f && degrees < 157.5f){
		direction = UP_RIGHT;
	}else if(degrees >= 157.5f && degrees < 202.5f){
		direction = RIGHT;
	}else if(degrees >= 202.5f && degrees < 247.5f){
		direction = DOWN_RIGHT;
	}else if(degrees >= 247.5f && degrees < 292.5f){
		direction = DOWN;
	}else{
		direction = DOWN_LEFT;
	}
	
	constrainedVector = GameActor::getNormalVectorFromDirection(direction);
	
	if(constrain){
		this->body->SetLinearVelocity(b2Vec2(constrainedVector.x*runSpeed*speedMod, constrainedVector.y*runSpeed*speedMod));
	}else{
		this->body->SetLinearVelocity(b2Vec2(unconstrainedVector.x*runSpeed*speedMod, unconstrainedVector.y*runSpeed*speedMod));
	}
	
	if(lastAngularVelocity != 0.0f && lastAngularVelocity == this->body->GetAngularVelocity()){
		this->body->SetAngularVelocity(0.0f);
	}
	lastAngularVelocity = this->body->GetAngularVelocity();
	lastVelocity = ccp(this->body->GetLinearVelocity().x, this->body->GetLinearVelocity().y);
}

void GameActor::stopRunning()
{
	this->body->SetLinearVelocity(b2Vec2(0.0f,0.0f));
}

CCPoint GameActor::getNormalVectorFromDirection(int dir)
{
	CCPoint v;
	if(dir == LEFT){
		v = ccp(-1,0);
	}else if(dir == UP_LEFT){
		v = ccp(-0.7071067812,0.7071067812);
	}else if(dir == UP){
		v = ccp(0,1);
	}else if(dir == UP_RIGHT){
		v = ccp(0.7071067812,0.7071067812);
	}else if(dir == RIGHT){
		v = ccp(1,0);
	}else if(dir == DOWN_RIGHT){
		v = ccp(0.7071067812,-0.7071067812);
	}else if(dir == DOWN){
		v = ccp(0,-1);
	}else if(dir == DOWN_LEFT){
		v = ccp(-0.7071067812,-0.7071067812);
	}
	return v;
}
void GameActor::release()
{
    waypoints->removeAllObjects();
    waypoints->release();

}
void GameActor::animateActor()
{
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();// [CCSpriteFrameCache sharedSpriteFrameCache];
    
	//Animation delay is inverse speed
	float delay = 0.5f/sqrt(pow(this->lastVelocity.x,2)+pow(this->lastVelocity.y,2));
	if(delay > 0.5f){ delay = 0.5f; }
   
	CCAnimation *animation =CCAnimation::create();
    animation->setDelayPerUnit(delay);
    
   // [[CCAnimation alloc] initWithName:@"gunman_anim" delay:delay];
    
    string dir;
	bool flipX = false;
	bool moving = true;
	
	if(sqrt(pow(this->lastVelocity.x,2)+pow(this->lastVelocity.y,2)) < 0.2f){ moving = false; }
    
	if(this->direction == LEFT){ dir = "right"; flipX = true; }
	else if(this->direction == UP_LEFT){ dir = "up_right"; flipX = true; }
	else if(this->direction == UP){ dir = "up"; }
	else if(this->direction == UP_RIGHT){ dir = "up_right"; }
	else if(this->direction == RIGHT){ dir = "right"; }
	else if(this->direction == DOWN_RIGHT){ dir = "down_right"; }
	else if(this->direction == DOWN){ dir = "down"; }
	else if(this->direction == DOWN_LEFT){ dir = "down_right"; flipX = true; }
    
    ((CCSprite*)this->sprite->getChildByTag(0))->setFlipX(flipX);
	//((CCSprite*)[this->sprite getChildByTag:0]).flipX = flipX;
   
    
	if(moving){
         
        animation->addSpriteFrame(cache->spriteFrameByName(CCString::createWithFormat("gunman_%s_run_%s_01.png",color.c_str(),dir.c_str())->getCString()));
        animation->addSpriteFrame(cache->spriteFrameByName(CCString::createWithFormat("gunman_%s_stand_%s.png",color.c_str(),dir.c_str())->getCString()));
        animation->addSpriteFrame(cache->spriteFrameByName(CCString::createWithFormat("gunman_%s_run_%s_02.png",color.c_str(),dir.c_str())->getCString()));
        animation->addSpriteFrame(cache->spriteFrameByName(CCString::createWithFormat("gunman_%s_stand_%s.png",color.c_str(),dir.c_str())->getCString()));
	 
	}else{
        animation->addSpriteFrame(cache->spriteFrameByName(CCString::createWithFormat("gunman_%s_stand_%s.png",color.c_str(),dir.c_str())->getCString()));
		 
	}
      
	//animateActor calls itself indefinitely
    this->sprite->getChildByTag(0)->runAction(CCSequence::create (CCAnimate::create(animation),CCCallFunc::create(this, callfunc_selector(GameActor::animateActor)),NULL));
     
     
}
void GameActor::timeShiftVectMagnitude(float seconds,CCPoint vect,float magnitude)
{
	b2Vec2 force = b2Vec2(vect.x*magnitude, vect.y*magnitude);
    
	body->SetLinearVelocity(body->GetLinearVelocity() + seconds * (1.0f/body->GetMass() * force));
    
	float clamp = clampf(1.0f - seconds * body->GetLinearDamping(), 0.0f, 1.0f);
	b2Vec2 v = body->GetLinearVelocity();
	body->SetLinearVelocity(b2Vec2(v.x*clamp, v.y*clamp));
}

