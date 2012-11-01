//
//  GameArea2D.cpp
//  Box2dWater
//
//  Created by Dave on 12-8-28.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#include "GameArea2D.h"

#include "BasicContactFilter.h"
GameArea2D::GameArea2D()
{
    Recipe::Recipe();
     
	bodiesToDestroy = new CCArray(5);
    postDestructionCallbacks = new CCArray(5)  ;
    bodiesToCreate =new CCArray(5);
    queuedActions = new CCArray(5);
    
    cameraZoom = 1.0f;
    lastMultiTouchZoomDistance = 0.0f;
    draggedToPoint = ccp(0,0);
    lastTouchedPoint = ccp(0,0);
    
    cameraState = GA_CAMERA_STATE_NONE;
    allTouches = new CCDictionary();

}
void GameArea2D::draw()
{
}

void GameArea2D::drawLayer()
{
}
void GameArea2D::step(float dt)
{
	//Update Physics
	int32 velocityIterations = 8;
	int32 positionIterations = 3;
	
	world->Step(dt, velocityIterations, positionIterations);
	
	//Set sprite positions by body positions
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL) {
			 GameObject *obj = (GameObject*)b->GetUserData();
           // CCLog("GameObject type = %d",obj->type());
//            if (obj->sprite!= NULL) {
//               obj->sprite->setPosition(CCPointMake(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) ); 
//            }
            
			//[obj.sprite setPosition:CGPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO)];
			//obj.sprite.rotation = -1 * CC_RADIANS_TO_DEGREES(b->GetAngle());
            if (obj->sprite!= NULL) {
                obj->sprite->setPosition(ccp( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
                 
            }
		}
	}
	
	//Process body destruction/creation
    this->destroyBodies();
    this->createBodies();
    this->runQueuedActions();
 
	
	/* Process camera */
    this->checkCameraBounds();
    this->processZoomStep();
    
 
}


CCLayer* GameArea2D::runRecipe()
{
 //   super->runRecipe();
    Recipe::runRecipe();
    
    CCSize screenSize =CCDirector::sharedDirector()->getWinSize();
	     
	//The gameNode is main node we'll attach everything to
	gameNode =  new CCNode();
     
	gameNode->setPosition(ccp(screenSize.width/2,screenSize.height/2));
    this->addChild(gameNode,1);
	gameNode->setContentSize( CCSizeMake(gameAreaSize.x*PTM_RATIO,gameAreaSize.y*PTM_RATIO));
    gameNode->setAnchorPoint(ccp(0.5f,0.5f));
 
    
	//Set Y gravity to 0
	b2Vec2 gravity;
	gravity.Set(0.0f, 0.0f);
	
	//Allow objects to sleep to save cycles
	bool doSleep = true;
	
	//Create our world
     
	world = new b2World(gravity);
    world->SetAllowSleeping(doSleep);
	world->SetContinuousPhysics(true);
	
	//Add contact filter and contact listener
	world->SetContactFilter(new basicContactFilter);
	world->SetContactListener(new basicContactListener);
	 
	//Set up debug drawing routine
	m_debugDraw = new GLESDebugDraw(32.0f );
	world->SetDebugDraw(m_debugDraw);
	
	//Initialize then show debug drawing
 
    this->initDebugDraw();
    this->showMinimalDebugDraw();
    /*
     //Add button to hide/show debug drawing
     CCMenuItemFont* swapDebugDrawMIF = [CCMenuItemFont itemFromString:@"Debug Draw" target:self selector:@selector(swapDebugDraw)];
     CCMenu *swapDebugDrawMenu = [CCMenu menuWithItems:swapDebugDrawMIF, nil];
     swapDebugDrawMenu.position = ccp( 260 , 20 );
     [self addChild:swapDebugDrawMenu z:5];
     */
	//Schedule our every tick method call
    this->schedule(schedule_selector(GameArea2D::step));
	
	return this;
}

























/* This is called from 'basicContactListener'. It will need to be overridden. */
void GameArea2D::handleCollisionWithObjAWithObjB(GameObject *objA, GameObject *objB)
{
	/** ABSTRACT **/
}

/* Destroy the world upon exit */
//- (void) dealloc
//{
//	delete world;
//	world = NULL;
//	
//	delete m_debugDraw;
//	
//	[allTouches release];
//	
//	[super dealloc];
//}

/* Debug information is drawn over everything */
void GameArea2D::initDebugDraw()
{
  
	debugDrawNode =  DebugDrawNode::createWithWorld(world);
    debugDrawNode->setPosition(ccp(0,0));
    gameNode->addChild(debugDrawNode,100000);
	 
}

/* When we show debug draw we add a number of flags to show specific information */
void GameArea2D::showDebugDraw()
{
	debugDraw  = true;
    
	uint32 flags = 0;
//	flags += b2DebugDraw::e_shapeBit;
//	flags += b2DebugDraw::e_jointBit;
//	flags += b2DebugDraw::e_aabbBit;
//	flags += b2DebugDraw::e_pairBit;
//	flags += b2DebugDraw::e_centerOfMassBit;
	m_debugDraw ->SetFlags(flags);
}

/* Minimal debug drawing only shows the shapeBit flag */
void GameArea2D::showMinimalDebugDraw()
{
	debugDraw = true;
     
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	m_debugDraw->SetFlags(flags);
}

/* Hide debug drawing */
void GameArea2D::hideDebugDraw()
{
	debugDraw = false;
    
	uint32 flags = 0;
	m_debugDraw->SetFlags(flags);
}

/* Swap debug draw callback */
void GameArea2D::swapDebugDraw()
{
	if(debugDraw){
        this->hideDebugDraw();
		 
	}else{
        this->showDebugDraw();
		 
	}
}

/* Add basic level boundary polygon. This is often overridden */
void GameArea2D::addLevelBoundaries()
{
	GameMisc *obj = new GameMisc();
	obj->gameArea = this;
	obj->setTag(GO_TAG_WALL);
   
	obj->bodyDef->type = b2_staticBody;
	obj->bodyDef->position.Set(0,0);
	obj->bodyDef->userData = obj;
	obj->body = world->CreateBody(obj->bodyDef);
//	
	obj->polygonShape = new b2PolygonShape();
	obj->fixtureDef->shape = obj->polygonShape;
	obj->fixtureDef->density = 1.0f;
	obj->fixtureDef->friction = 1.0f;
	obj->fixtureDef->restitution = 0.0f;
//
	obj->polygonShape->SetAsEdge(b2Vec2(0,0), b2Vec2(gameAreaSize.x,0));
	obj->body->CreateFixture(obj->fixtureDef);
    
	obj->polygonShape->SetAsEdge(b2Vec2(0,gameAreaSize.y), b2Vec2(0,0));
	obj->body->CreateFixture(obj->fixtureDef);
	
	obj->polygonShape->SetAsEdge(b2Vec2(0,gameAreaSize.y), b2Vec2(gameAreaSize.x,gameAreaSize.y));
	obj->body->CreateFixture(obj->fixtureDef);
	
	obj->polygonShape->SetAsEdge(b2Vec2(gameAreaSize.x,gameAreaSize.y), b2Vec2(gameAreaSize.x,0));
	obj->body->CreateFixture(obj->fixtureDef);
	
	levelBoundary = obj;
}

void  GameArea2D::addRandomPolygons(int num)
{
	for(int i=0; i<num; i++){
		float x = (float)(arc4random()%((int)gameAreaSize.x*PTM_RATIO));
		float y = (float)(arc4random()%((int)gameAreaSize.y*PTM_RATIO));
        
      //  CCLog("addRandomPolygons %d = x=%f y=%f",i,x,y);
		this->addPolygonAtPoint(ccp(x,y));
		 
	}
}

void GameArea2D::addRandomBoxes(int num)
{
	for(int i=0; i<num; i++){
		float x = (float)(arc4random()%((int)gameAreaSize.x*PTM_RATIO));
		float y = (float)(arc4random()%((int)gameAreaSize.y*PTM_RATIO));
		
        this->addBoxAtPointAndSize(ccp(x,y), ccp((float)(arc4random()%200)+100.0f,(float)(arc4random()%50)+30.0f));
		 
	}
}

/* Adding a polygon */
void GameArea2D::addPolygonAtPoint(CCPoint p)
{
	//Random collection of points
	CCArray *points = CCArray::create();
	for(int i=0; i<(arc4random()%5+3); i++){
		float x = (float)(arc4random()%100)+10;
		float y = (float)(arc4random()%100)+10;
		Vector3D *v = Vector3D::xyz(x,y,0);
       // CCLog("points->v3D{x=%f,y=%f,z=%f}",v->x,v->y,v->z);
        points->addObject(v );
		 
	}
//    points->addObject(Vector3D::xyz(11.000000,87.000000,0));
//    points->addObject(Vector3D::xyz(93.000000,107.000000,0));
//    points->addObject(Vector3D::xyz(26.000000,15.000000,0));
//    points->addObject(Vector3D::xyz(24.000000,37.000000,0));
//    points->addObject(Vector3D::xyz(23.000000,82.000000,0));
//    points->addObject(Vector3D::xyz(12.000000,30.000000,0));
//    points->addObject(Vector3D::xyz(82.000000,68.000000,0));
     
	
	//Convex polygon points
	CCArray *convexPolygon = GameHelper::convexHull(points);
	
	//Convex Polygon
	float polygonSize = 0.05f;
    
	int32 numVerts = convexPolygon->count();
	b2Vec2 *vertices;
	vertices = new b2Vec2[convexPolygon->count()];
	
	CCArray *vertexArray = CCArray::create();
	
	CCPoint maxSize = ccp(0,0);
	for(int i=0; i<convexPolygon->count(); i++){
		Vector3D *v =(Vector3D*)convexPolygon->objectAtIndex(i);
       // CCLog("convexPolygon->v3D{x=%f,y=%f,z=%f}",v->x,v->y,v->z);
		vertices[i].Set(v->x*polygonSize,v->y*polygonSize);
        CCPoint p = ccp(v->x*PTM_RATIO*polygonSize, v->y*PTM_RATIO*polygonSize);
        vertexArray->addObject(Vector3D::xyz(p.x,p.y,0) );
        
		//[vertexArray addObject:[NSValue valueWithCGPoint:ccp(v.x*PTM_RATIO*polygonSize, v.y*PTM_RATIO*polygonSize)]];
		
		//Figure out max polygon size
		if(maxSize.x < v->x*polygonSize){ maxSize.x = v->x*polygonSize; }
		if(maxSize.y < v->y*polygonSize){ maxSize.y = v->y*polygonSize; }
	}
	
	//Keep polygon in game area
	if(p.x/PTM_RATIO + maxSize.x > gameAreaSize.x){ p.x = (gameAreaSize.x - maxSize.x)*PTM_RATIO; }
	if(p.y/PTM_RATIO + maxSize.y > gameAreaSize.y){ p.y = (gameAreaSize.y - maxSize.y)*PTM_RATIO; }
	if(p.x < 0){ p.x = 0; }
	if(p.y < 0){ p.y = 0; }
    
	GameMisc *obj = new GameMisc();
	obj->gameArea = this;
	obj->setTag( GO_TAG_WALL)  ;
    
	obj->bodyDef->type = b2_staticBody;
	obj->bodyDef->position.Set(p.x/PTM_RATIO, p.y/PTM_RATIO);
	obj->bodyDef->userData = obj;
	obj->body = world->CreateBody(obj->bodyDef);
	
	obj->polygonShape = new b2PolygonShape();
	obj->polygonShape->Set(vertices, numVerts);
	obj->fixtureDef->shape = obj->polygonShape;
	
	obj->body->CreateFixture(obj->fixtureDef);
}

/* Adding a polygon */
void GameArea2D::addBoxAtPointAndSize(CCPoint p,CCPoint s)
{
	//Random collection of points
	CCArray *points = CCArray::create();
	float x = s.x; float y = s.y;
	points->addObject(Vector3D::xyz(0, 0, 0));
    points->addObject(Vector3D::xyz(x, 0, 0));
    points->addObject(Vector3D::xyz(x, y, 0));
    points->addObject(Vector3D::xyz(0, y, 0));
    
	float polygonSize = 0.05f;
    
	int32 numVerts = points->count();
	b2Vec2 *vertices;
	vertices = new b2Vec2[points->count()];
	
	CCArray *vertexArray = CCArray::create();
	
	CCPoint maxSize = ccp(0,0);
	for(int i=0; i<points->count(); i++){
		Vector3D *v = (Vector3D*)points->objectAtIndex(i);//[points objectAtIndex:i];
		vertices[i].Set(v->x*polygonSize, v->y*polygonSize);
        
        CCPoint p1 = ccp(v->x*PTM_RATIO*polygonSize, v->y*PTM_RATIO*polygonSize);
        vertexArray->addObject(Vector3D::xyz(p1.x,p1.y,0) );
        
        //Figure out max polygon size
		if(maxSize.x < v->x*polygonSize){ maxSize.x = v->x*polygonSize; }
		if(maxSize.y < v->y*polygonSize){ maxSize.y = v->y*polygonSize; }
	}
	
	//Keep polygon in game area
	if(p.x/PTM_RATIO + maxSize.x > gameAreaSize.x){ p.x = (gameAreaSize.x - maxSize.x)*PTM_RATIO; }
	if(p.y/PTM_RATIO + maxSize.y > gameAreaSize.y){ p.y = (gameAreaSize.y - maxSize.y)*PTM_RATIO; }
	if(p.x < 0){ p.x = 0; }
	if(p.y < 0){ p.y = 0; }
    
	GameMisc *obj = new GameMisc();
	obj->gameArea = this;
	obj->setTag(GO_TAG_WALL)  ;
   
	obj->bodyDef->type = b2_staticBody;
	obj->bodyDef->position.Set(p.x/PTM_RATIO, p.y/PTM_RATIO);
	obj->bodyDef->userData = obj;
	obj->body = world->CreateBody(obj->bodyDef);
	
	obj->polygonShape = new b2PolygonShape();
	obj->polygonShape->Set(vertices, numVerts);
	obj->fixtureDef->shape = obj->polygonShape;
	obj->fixtureDef->restitution = 0.0f;
	obj->fixtureDef->friction = 1.0f;
	
	obj->body->CreateFixture(obj->fixtureDef);
}

/* Mark a body for destruction */
void GameArea2D::markBodyForDestruction(GameObject* obj)
{
    
    bodiesToDestroy->addObject(obj);
     
	//[bodiesToDestroy addObject:[NSValue valueWithPointer:obj]];
}

/* Destroy queued bodies */

//TODO - Can we limit the number of created and destroyed bodies per tick?
//       This might be messed up by the fact that we are using GameObject pointers. If a GameObject has a new body created before its old
//       body is destroyed this creates a zombie body somewhere in memory.
void GameArea2D::destroyBodies()
{
	for(int i=0;i<bodiesToDestroy->count();i++){
        GameObject* obj = (GameObject*)bodiesToDestroy->objectAtIndex(i);
		if(obj && obj->body && !obj->markedForDestruction){
			obj->body->SetTransform(b2Vec2(0,0),0);
			world->DestroyBody(obj->body);
			obj->markedForDestruction = true;
		}
	}
    bodiesToDestroy->removeAllObjects();
	 
	
	//Call all game object callbacks
    for(int i=0;i<postDestructionCallbacks->count();i++){
        ObjectCallback* goc = (ObjectCallback*)bodiesToDestroy->objectAtIndex(i);
        
        CCCallFunc *func = CCCallFunc::create(goc->obj,goc->callbackfun);
        func->execute();
       
     
    }
    postDestructionCallbacks->removeAllObjects();
 
}

/* Mark a body for creation */
void GameArea2D::markBodyForCreation(GameObject* obj)
{
	//[bodiesToCreate addObject:[NSValue valueWithPointer:obj]];
    bodiesToCreate->addObject(obj);
}

/* Create all queued bodies */
void GameArea2D::createBodies()
{
    for(int i=0;i<bodiesToCreate->count();i++){
        GameObject* obj = (GameObject*)bodiesToCreate->objectAtIndex(i);
        obj->body = world->CreateBody(obj->bodyDef);
		obj->body->CreateFixture(obj->fixtureDef);

		 
	}
	 
	bodiesToCreate ->removeAllObjects();
}

/* Run any queued actions after creation/destruction */
void GameArea2D::runQueuedActions()
{
    for(int i=0;i<queuedActions->count();i++){
        QueuedAction* qa = (QueuedAction*)queuedActions->objectAtIndex(i);
        GameObject *gameObject = (GameObject*)qa->gameObject;
        CCAction *action = (CCAction*)qa->action;
        
        gameObject->runAction(action);
    }
    queuedActions->removeAllObjects();
	 
}

/* Drawing */
void drawLayer()
{
	/* ABSTRACT */
}

/* A camera bound limiting routine */
bool GameArea2D::checkCameraBoundsWithFailPosition(CCPoint failPosition)
{
     
	CCSize screenSize =CCDirector::sharedDirector()->getWinSize();
	bool passed = true;
	
	//Return true for now
	return passed;
	
	float fsx = (gameAreaSize.x/2)*cameraZoom;
	float fsy = (gameAreaSize.y/2)*cameraZoom;
	float ssx = screenSize.width;
	float ssy = screenSize.height;
    
	if( gameNode-> getPosition().y < -(fsy - ssy) ) {
		failPosition.y = -(fsy - ssy);
		passed = false;
	}else if( gameNode-> getPosition().y > fsy) {
		failPosition.y = fsy;
		passed = false;
	}else{ //Passed
		failPosition.y = gameNode-> getPosition().y;
	}
	
	if( gameNode-> getPosition().x < -(fsx - ssx) ) {
		failPosition.x = -(fsx - ssx);
		passed = false;
	}else if( gameNode-> getPosition().x > fsx) {
		failPosition.x = fsx;
		passed = false;
	}else { //Passed
		failPosition.x = gameNode-> getPosition().x;
	}
	
	return passed;
}

/* Check camera bounds and reset them if neccessary */
bool GameArea2D::checkCameraBounds()
{
	CCPoint failPosition;
	bool cameraInBounds = this->checkCameraBoundsWithFailPosition(failPosition);
     
	if(!cameraInBounds){ //Test failed.
        gameNode->setPosition(failPosition);
	}
	
	return cameraInBounds;
}

/* Properly set the camera position using 'gameNode' */
void GameArea2D::setCameraPosition(CCPoint position)
{
     gameNode->setPosition(this->convertCamera(position));
	 
}

/* Convert a camera position */
CCPoint GameArea2D::convertCamera(CCPoint p) {
	 
    CCSize screenSize =CCDirector::sharedDirector()->getWinSize();
	CCPoint newPoint = ccp(p.x*-1, p.y*-1);
	
	//Standard Camera Adjustment
	newPoint = ccp(newPoint.x + (screenSize.width/2) + (gameAreaSize.x/2), newPoint.y + (screenSize.height/2) + (gameAreaSize.y/2));
	
	//Zoom adjustment
	newPoint = ccp(newPoint.x*cameraZoom, newPoint.y*cameraZoom);
	
	return newPoint;
}
/* Set the camera zoom with limits */
void GameArea2D::setCameraZoom(float zoom)
{
	if(zoom < 0.1f){ zoom = 0.1f; }	//Lower limit
	if(zoom > 4.0f){ zoom = 4.0f; }	//Upper limit
	 
    gameNode->setScale( zoom);
	cameraZoom = zoom;
}
/* Set cameraZoomTo and cameraZoomSpeed */
void GameArea2D::zoomToWithSpeed(float zoom,float speed){	//Speed must be between 0 and 1 exclusive.
	cameraZoomTo = zoom;
	cameraZoomSpeed = speed;
}
/* Process a zoom step */
void GameArea2D::processZoomStep()
{
	float newZoom = cameraZoom;
	if(newZoom != cameraZoomTo){
		if(newZoom < cameraZoomTo){
			newZoom += cameraZoomSpeed;
			if(newZoom > cameraZoomTo){ newZoom = cameraZoomTo; }
		}else if(newZoom > cameraZoomTo){
			newZoom -= cameraZoomSpeed;
			if(newZoom < cameraZoomTo){ newZoom = cameraZoomTo; }
		}
        this->setCameraZoom(newZoom);
		 
	}
}

/* Process touches */
void GameArea2D::ccTouchesBegan(CCSet *touches ,CCEvent *event) {
	bool hudTouched = false;
	hudTouched = this->hudBegan(touches, event); 
    CCSetIterator it;
    CCTouch* touch;
    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        if(!touch)
            break;
        ;
     //   const  string  hashStr = "touch_"+touch->m_uID;
         CCLog("->>%s<<-",CCString::createWithFormat("touch_%d", touch->m_uID)->getCString());
        
        
        allTouches->setObject(touch,CCString::createWithFormat("touch_%d", touch->m_uID)->getCString());
    }
	 
	
	if(!hudTouched){
		if(touches->count() == 1 && allTouches->count() == 1){
			CCTouch *touch = (CCTouch*)touches->anyObject();
			CCPoint point = touch->getLocationInView();
			point = CCDirector::sharedDirector()->convertToGL(point); 
			
			draggedToPoint = ccp(0,0);
			lastTouchedPoint = point;
		}else if(touches->count() == 2){
			lastMultiTouchZoomDistance = 0.0f;
		}
	}
}
void GameArea2D::ccTouchesMoved(CCSet *touches ,CCEvent *event)
{
     
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
	bool hudTouched = false;
	hudTouched = this->hudMoved(touches, event);
	
	if(!hudTouched){
		if(touches->count()  == 1 && allTouches->count()  == 1){
			CCTouch *touch = (CCTouch*)touches->anyObject();
			CCPoint point = touch->getLocationInView();
			point = CCDirector::sharedDirector()->convertToGL(point);
			
			//Process camera movement
			CCPoint lastTouchedDistance = ccp(point.x - lastTouchedPoint.x, point.y - lastTouchedPoint.y);
			draggedToPoint = ccp(draggedToPoint.x + lastTouchedDistance.x, draggedToPoint.y + lastTouchedDistance.y);
			
			//Should we start panning?
			if(cameraState == GA_CAMERA_STATE_NONE && this->distanceP1toP2(ccp(0,0), draggedToPoint) > TAP_VS_DRAG_DIST){
				cameraState = GA_CAMERA_STATE_PANNING;
			}
			
			//Pan if we are already panning
			if(cameraState == GA_CAMERA_STATE_PANNING){
				//Move camera
				gameNode->setAnchorPoint(ccp(gameNode->getAnchorPoint().x - lastTouchedDistance.x/cameraZoom/gameNode->getContentSize().width,
                                              gameNode->getAnchorPoint().y - lastTouchedDistance.y/cameraZoom/gameNode->getContentSize().height));
			}
			lastTouchedPoint = point;
		}else if(touches->count() == 2){
            CCSetIterator it;
            CCTouch* touch1;
            CCTouch* touch2;
            int ke = 1;
            for( it = touches->begin(); it != touches->end(); it++)
            {
                 
                if (ke==1)
                {
                     touch1 = (CCTouch*)(*it);
                }else if(ke==2)
                {
                     touch2 = (CCTouch*)(*it);
                }
                ke++;
                  
                if (ke>2) {
                    break;
                }
            }
            
			 
			CCPoint point1 = touch1->getLocationInView();
			CCPoint point2 = touch2->getLocationInView();
            
			//Process camera zooming
			cameraState = GA_CAMERA_STATE_ZOOMING;
            
			float currentDistance = this->distanceP1toP2(point1, point2);
            
			if(lastMultiTouchZoomDistance == 0){
				lastMultiTouchZoomDistance = currentDistance;
			}else{
				float difference = currentDistance - lastMultiTouchZoomDistance;
				float newZoom = (cameraZoom + (difference/screenSize.height)*cameraZoom);
                this->setCameraZoom(newZoom);
				lastMultiTouchZoomDistance = currentDistance;
			}
		}
	}
}
void GameArea2D::ccTouchesEnded(CCSet *touches ,CCEvent *event)
{
	bool hudTouched = false;
	hudTouched = this->hudEnded(touches, event);
	
	//Store all touches
    
    
    CCSetIterator it;
    CCTouch* touch;
    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        if(!touch)
            break;
         
        allTouches->removeObjectForKey(CCString::createWithFormat("touch_%d", touch->m_uID)->getCString());
    } 
    
	if(!hudTouched){
		CCTouch *touch =(CCTouch*) touches->anyObject();
        
		CCPoint point = touch->getLocationInView();
        point = CCDirector::sharedDirector()->convertToGL(point);
        
		if(cameraState == GA_CAMERA_STATE_NONE){
            this->tapWithPoint(point);
			cameraState = GA_CAMERA_STATE_NONE;
		}else if(cameraState == GA_CAMERA_STATE_PANNING){
			lastTouchedPoint = point;
			draggedToPoint = ccp(0,0);
			cameraState = GA_CAMERA_STATE_NONE;
		}else if(cameraState == GA_CAMERA_STATE_ZOOMING && allTouches->count() == 0){
			lastMultiTouchZoomDistance = 0.0f;
			cameraState = GA_CAMERA_STATE_NONE;
		}
	}
}

//Converts a touch coordinate to a game coordinate
CCPoint GameArea2D::convertTouchCoord(CCPoint touchPoint)
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
	 
	
	float cz = cameraZoom;
	CCPoint tp = touchPoint;
	CCPoint ap = gameNode->getAnchorPoint();
	CCPoint cs = ccp(gameNode->getContentSize().width, gameNode->getContentSize().height);
	CCPoint ss = ccp(screenSize.width, screenSize.height);
	
	float x = ((tp.x - ss.x/2) / cz) + (ap.x - 0.5f) * cs.x + cs.x/2;
	float y = ((tp.y - ss.y/2) / cz) + (ap.y - 0.5f) * cs.y + cs.y/2;
	
	return ccp(x,y);
}
//Converts a game coordinate to a touch coordinate
CCPoint GameArea2D::convertGameCoord(CCPoint gamePoint)
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    float cz = cameraZoom;
	CCPoint gp = gamePoint;
	CCPoint ap = gameNode->getAnchorPoint();
	CCPoint cs = ccp(gameNode->getContentSize().width, gameNode->getContentSize().height);
	CCPoint ss = ccp(screenSize.width, screenSize.height);
	
	float x = gp.x*cz - cs.x*(ap.x-0.5f)*cz - cs.x/2 + ss.x/2;
	float y = gp.y*cz - cs.y*(ap.y-0.5f)*cz - cs.y/2 + ss.y/2;
    
	return ccp(x,y);
}

void GameArea2D::centerCameraOnGameCoord(CCPoint gamePoint)
{
	gameNode->setAnchorPoint(  ccp(gamePoint.x/gameNode->getContentSize().width, gamePoint.y/gameNode->getContentSize().height)) ;
}

bool GameArea2D::hudBegan(CCSet *touches,CCEvent *event)
{
	/* ABSTRACT */
	return false;
}
 bool GameArea2D::hudMoved(CCSet *touches ,CCEvent *event)
{
    /* ABSTRACT */
	return false;
}
bool GameArea2D::hudEnded(CCSet *touches ,CCEvent *event)
{
    /* ABSTRACT */
	return false;
}
 
void GameArea2D::tapWithPoint(CCPoint point) {
	//ABSTRACT
}

float GameArea2D::distanceP1toP2(CCPoint p1,CCPoint p2) {
	return sqrtf( powf( (p1.x-p2.x) ,2) + powf( (p1.y-p2.y) ,2) );
}



