//
//  Tiled.cpp
//  A*Path
//
//  Created by Dave on 12-10-14.
//
//

#include "Tiled.h"
CCLayer* Tiled::runRecipe()
{
    //Load TMX tilemap file
    tileMap = CCTMXTiledMap::create("tilemap.tmx");
    //Set game area size based on tilemap size
    this->setGameAreaSize();
    //Superclass initialization and message
    GameArea2D::runRecipe();
   
    this->showMessage("Use the DPad to move the actor around.");
	 
    
	//Set initial variables
	spawnPoint = NULL;
	cameraZoom = 1.0f;
    
	//Make sure 2D projection is set
    CCDirector::sharedDirector()->setProjection(kCCDirectorProjection2D);
	 
    
	//Add tile map
    gameNode->addChild(tileMap,0);
	
    
	/* Re-order layers according to their Y value. This creates isometric depth. */
	float mw = tileMap->getMapSize().width; float mh = tileMap->getMapSize().height;
	float tw = tileMap->getTileSize().width; float th = tileMap->getTileSize().height;
    
	//Our layers
	CCTMXLayer *collidableLayer = tileMap->layerNamed("Collidable");
	CCTMXLayer *ground = tileMap->layerNamed("Ground");
	CCTMXLayer *wall = tileMap->layerNamed("Wall");
    
	//Gather all the layers into a container
     
	CCDictionary *layersToReorder =  CCDictionary::create();
    CCArray *childrens = tileMap->getChildren();
    for (int i = 0; i< childrens->count(); i++) {
        CCTMXLayer* child = (CCTMXLayer*)childrens->objectAtIndex(i);
        //Skip tiles marked "Collidable", "Ground" and "Wall"
		if(child == ground){ continue; }
		else if(child == wall){ continue; }
		else if(child == collidableLayer){ continue; }
		
		//Gather all the layers
		for(float x=0; x<mw; x+=1){
			for(float y=mh-1; y>=0; y-=1){
                
				CCSprite *childTile = child->tileAt(ccp(x,y));
                
				CCSprite *collideTile = collidableLayer->tileAt(ccp(x,y));
				
				if(childTile && collideTile){
                  
                    
                    layersToReorder->setObject(CCString::createWithFormat("%f",y),CCString::createWithFormat("%s",child->getLayerName())->getCString() );
					 
					x=mw; y=-1;
				}
			}
		}
        
	}
     
	
	//Re-order gathered layers
    CCArray *layersToReorderKey = layersToReorder->allKeys();
    for (int i=0; i<layersToReorderKey->count(); i++) {
        
        CCString* str = (CCString*)layersToReorderKey->objectAtIndex(i);
        CCLog("------>%s",str->getCString());
        CCLog("------>%f",((CCString*)layersToReorder->objectForKey(str->getCString()))->floatValue());
        tileMap->reorderChild(tileMap->layerNamed(str->getCString()),((CCString*)layersToReorder->objectForKey(str->getCString()))->floatValue());
    }
//    
//	for(id key in layersToReorder){
//		NSString *str = (NSString*)key;
//		[tileMap reorderChild:[tileMap layerNamed:str] z:[[layersToReorder objectForKey:key] floatValue]];
//	}
	
	//Set the ground to z=0
    tileMap->reorderChild(ground, 0);
	//[tileMap reorderChild:ground z:0];
    
	//Add Box2D boxes to represent all layers marked "Collidable"
    
	for(float x=0; x<mw; x+=1){
		for(float y=0; y<mh; y+=1){
            if(collidableLayer->tileAt(ccp(x,y))){
                this->addBoxAtPointAndSize( ccp(x*tw, mh*th - y*th), ccp(tw/2,th/2));
				
			}
		}
	}
	
	//Remove the "Collidable" layer art as its only an indicator for the level editor
    tileMap->removeChild(collidableLayer, true);
	//[tileMap removeChild:collidableLayer cleanup:YES];
    
	//Add DPad
	//[self addDPad];
    this->addDPad();
    
    this->hideDebugDraw();
	//[self hideDebugDraw];
    
	//Create Actor
    this->addActor();
	//[self addActor];
    
	//Create level boundaries
    this->addLevelBoundaries();
	//[self addLevelBoundaries];
    
	//This sets off a chain reaction
	if(actor){
        actor->animateActor();
    }
    
    this->schedule(schedule_selector(Tiled::step));
	//[self schedule:@selector(step:)];
    
	return this;
}

void Tiled::step(float dt)
{
    GameArea2D::step(dt);
	//[super step:dt];
	
	//Process DPad
	if(dPad->pressed){
		//[actor runWithVector:dPad.pressedVector withSpeedMod:1.0f withConstrain:NO];
		actor->body->ApplyForce(b2Vec2(dPad->pressedVector.x*15.0f, dPad->pressedVector.y*15.0f), actor->body->GetPosition());
		actor->body->SetAngularVelocity(0.0f);
		CCPoint movementVector = ccp(actor->body->GetLinearVelocity().x, actor->body->GetLinearVelocity().y);
        
		actor->body->SetTransform(actor->body->GetPosition(), -1 * GameHelper::vectorToRadians(movementVector)+ PI_CONSTANT/2);
	}else{
        actor->stopRunning();
		//[actor stopRunning];
	}
	
	//Store last gunman direction
	if(dPad->direction != NO_DIRECTION){
		actor->direction = dPad->direction;
	}
	
	//Flip the animated sprite if neccessary
	if(dPad->direction == LEFT || dPad->direction == UP_LEFT || dPad->direction == DOWN_LEFT){
        ((CCSprite*)actor->sprite->getChildByTag(0))->setFlipX(true);
		//((CCSprite*)[actor->sprite getChildByTag:0]).flipX = true;
	}else if(dPad->direction != NO_DIRECTION){
        ((CCSprite*)actor->sprite->getChildByTag(0))->setFlipX(false);
		//((CCSprite*)[actor->sprite getChildByTag:0]).flipX = false;
	}
	
	//Store last velocity
	actor->lastVelocity = ccp(actor->body->GetLinearVelocity().x, actor->body->GetLinearVelocity().y);
	
	//Re-order the actor
	float mh = tileMap->getMapSize().height;
	float th = tileMap->getTileSize().height;
    
	CCPoint p = actor->sprite->getPosition();
	float z = -(p.y/th) + mh;
    tileMap->reorderChild(actor->sprite, z);
	//[tileMap reorderChild:actor.sprite z:z ];
    this->cameraFollowActor();
	//[self cameraFollowActor];
}

void Tiled::cameraFollowActor(){
	//Follow the actor with the camera
	CCPoint actorPosition = ccp(actor->body->GetPosition().x*PTM_RATIO, actor->body->GetPosition().y*PTM_RATIO);
    this->centerCameraOnGameCoord(actorPosition);
}

//void Tiled::setCameraZoom(float zoom) {
//	//Disable zooming
//}

void Tiled::setGameAreaSize() {
	//Set gameAreaSize based on tileMap size
      
	gameAreaSize = ccp((tileMap->getMapSize().width * tileMap->getTileSize().width)/PTM_RATIO,(tileMap->getMapSize().height * tileMap->getTileSize().height)/PTM_RATIO);	//Box2d units
}

void Tiled::addDPad() {
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("dpad_buttons.plist");
	 
	dPad = new DPad();
	dPad->setPosition(ccp(75,75));
	dPad->setOpacity(100);
    this->addChild(dPad,5);
	 
}

void Tiled::addActor() {
	//Get spawn point from tile object named "SpawnPoint"
	if(!spawnPoint){
		CCTMXObjectGroup *objects = tileMap->objectGroupNamed("Objects");
        CCAssert(objects!=NULL, "'Objects' object group not found");
      
        CCDictionary *sp = objects->objectNamed("SpawnPoint");
		 
		CCAssert(sp != NULL, "SpawnPoint object not found");
        
		int x = sp->valueForKey("x")->intValue();
		int y = sp->valueForKey("y")->intValue();
        
		spawnPoint = Vector3D::xyz(x, y, 0);
	}
    
	//Add actor
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("gunman_lightblue.plist");
	
	actor = new GameActor();// [[GameActor alloc] init];
	actor->gameArea = this;
    
	actor->sprite = CCSprite::create("blank.png");
	actor->sprite->setScale(0.5f);
    actor->sprite->addChild(CCSprite::create("blank.png"),1,0);
	
	actor->bodyDef->type = b2_dynamicBody;
	actor->bodyDef->position.Set(spawnPoint->x/PTM_RATIO, spawnPoint->y/PTM_RATIO);
    CCLog("spawnPoint = %f,%f",(spawnPoint->x/PTM_RATIO), (spawnPoint->y/PTM_RATIO));
	actor->bodyDef->userData = actor;
    
	actor->body = world->CreateBody(actor->bodyDef);
	
	actor->circleShape = new b2CircleShape();
	actor->circleShape->m_radius = 0.5f;
	actor->fixtureDef->shape = actor->circleShape;
	
	actor->body->CreateFixture(actor->fixtureDef);
	
	actor->body->SetLinearDamping(2.0f);
	//CCAssert(tileMap->layerNamed("0")!=NULL, "is not null,");
   
    //tileMap->addChild(actor->sprite,tileMap->layerNamed("0")->getVertexZ());
    
    tileMap->addChild(actor->sprite,5);
    //actor->sprite->setPosition(ccp(spawnPoint->x, spawnPoint->y));
     //tileMap->addChild(CCSprite::create("end_button.png"),10);
    
	//[tileMap addChild:actor.sprite z:[[tileMap layerNamed:@"0"] vertexZ]];
}

bool Tiled::hudBegan(CCSet *touches,CCEvent *event){
	bool hudTouched = false;
    dPad->ccTouchesBegan(touches, event); 
	if(dPad->pressed || touches->count() == 1){ hudTouched = true; }
	return hudTouched;
}

bool Tiled::hudMoved(cocos2d::CCSet *touches, cocos2d::CCEvent *event) {
	bool hudTouched = false;
    dPad->ccTouchesMoved(touches, event);
	 
	if(dPad->pressed || touches->count() == 1){ hudTouched = true; }
	return hudTouched;
}

bool Tiled::hudEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *event) {
	bool hudTouched = false;
    dPad->ccTouchesEnded(touches, event);
	 
	if(dPad->pressed || touches->count() == 1){ hudTouched = true; }
	return hudTouched;
}

void Tiled::cleanRecipe() {
    GameArea2D::cleanRecipe();
	 
}
