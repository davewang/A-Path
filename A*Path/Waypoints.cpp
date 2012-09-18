//
//  Waypoints.cpp
//  Box2dWater
//
//  Created by Dave on 12-8-30.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#include "Waypoints.h"
 
CCLayer* Waypoints::runRecipe()
{
	//Set game area size
	gameAreaSize = ccp(1000/PTM_RATIO,600/PTM_RATIO);	//Box2d units
    
	//Superclass initialization and message
    GameArea2D::runRecipe();
    message->setString("Tap the screen move the actor to that point.\nTap again to queue another waypoint.");
	 
    
	//Add polygons
    this->addRandomPolygons(10);
	 
	
	//Create Actor
    this->addActor();
 
	
	//Add draw layer
    this->addDrawLayer();
 
	
	//Create level boundaries
    this->addLevelBoundaries();
 
	
	//Initial variables
	cameraZoom = 0.45f;
    
	//Step method
    this->schedule(schedule_selector(Waypoints::step));
    
	return this;
}

void Waypoints::step(float dt)
{
 
    GameArea2D::step(dt);
	
	//Process actor waypoints
    actor->processWaypoints();
	
	//Turn actor toward waypoints
	if(actor->waypoints->count() > 0){
		CCPoint movementVector = ccp(actor->body->GetLinearVelocity().x, actor->body->GetLinearVelocity().y);
       
		actor->body->SetTransform(actor->body->GetPosition(), -1 *  GameHelper::vectorToRadians(movementVector ) + PI_CONSTANT/2);
	}
}

/* Add actor to scene */
void Waypoints::addActor(){
    
    
    actor = new GameActor();
    actor->gameArea = this;
    
    
	
	actor->bodyDef->type = b2_dynamicBody;
	actor->bodyDef->position.Set(240/PTM_RATIO,160/PTM_RATIO);
	actor->bodyDef->userData = actor;
    
	actor->body = world->CreateBody(actor->bodyDef);
	
	actor->circleShape = new b2CircleShape();
	actor->circleShape->m_radius = 0.5f;
	actor->fixtureDef->shape = actor->circleShape;
	actor->fixtureDef->isSensor = false;
	
	actor->body->CreateFixture(actor->fixtureDef);
}

/* Draw all waypoint lines */
void Waypoints::drawLayer()
{
    glColorMask(255,255,0,32);
	//glColor4ub(255,255,0,32);
 
	CCPoint actorPosition = ccp(actor->body->GetPosition().x*PTM_RATIO, actor->body->GetPosition().y*PTM_RATIO);
   // CCLog("actor->waypoints->count() %d",actor->waypoints->count());
	if(actor->waypoints->count() == 1){
       
		GameWaypoint *gw = (GameWaypoint*)actor->waypoints->objectAtIndex(0);
		ccDrawLine(actorPosition, gw->position);
	}else if(actor->waypoints->count() > 1){
        for(int i=0; i<actor->waypoints->count()- 1; i++){
			GameWaypoint *gw = (GameWaypoint*)actor->waypoints->objectAtIndex(i);
			GameWaypoint *gwNext = (GameWaypoint*)actor->waypoints->objectAtIndex(i+1);
			
			if(i == 0){
				//From actor to first waypoint
				ccDrawLine(actorPosition, gw->position);
				ccDrawLine(gw->position, gwNext->position);
			}else{
				//From this waypoint to next one
				ccDrawLine(gw->position, gwNext->position);
			}
		}
	}
	 glColorMask(255,255,255,255);
	 
}

/* Add extra drawing layer */
void Waypoints::addDrawLayer()
{
	_drawLayer = new DrawLayer();
	_drawLayer->setPosition(ccp(0,0)) ;
	_drawLayer->recipe = this;
    gameNode->addChild(_drawLayer,10);
	 
}

/* Add a new waypoint when you touch the screen */
void Waypoints::tapWithPoint(CCPoint point)
{
    
	ObjectCallback *goc1 = ObjectCallback::createWithObjectWithCallback(this,callfunc_selector(Waypoints::movingToWaypoint));
    //ObjectCallback::createWithObjectWithCallback(this, "movingToWaypoint");
	ObjectCallback *goc2 =  ObjectCallback::createWithObjectWithCallback(this,callfunc_selector(Waypoints::reachedWaypoint));//ObjectCallback::createWithObjectWithCallback(this, "reachedWaypoint");
    
    GameWaypoint *wp = GameWaypoint::createWithPositionWithSpeedMod(this->convertTouchCoord(point), 1.0f);
	 
	wp->preCallback = goc1;
	wp->postCallback = goc2;
    actor->addWaypoint(wp);
	 
}

/* WP message callbacks */
void Waypoints::movingToWaypoint()
{
    this->showMessage("Moving to WP" );
	 
}

void Waypoints::reachedWaypoint()
{
    this->showMessage("Reached WP" );
}
