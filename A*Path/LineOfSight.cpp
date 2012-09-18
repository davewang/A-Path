//
//  LineOfSight.cpp
//  A*Path
//
//  Created by Dave on 12-9-11.
//
//

#include "LineOfSight.h"

CCLayer* LineOfSight::runRecipe() {
	//Set game area size
    this->setGameAreaSize();
	//Superclass initalization and message
    GameArea2D::runRecipe();
	//[message setString:@"Use the DPad to avoid the AI actors."];
    message->setString("Use the DPad to avoid the AI actors." );
	//Add DPad
	this->addDPad();
    
	//Add random geometry
	this->addRandomPolygons(5);
    
	//Create Actor
	this->addActor();
	
	//Create enemies
	this->addEnemies();
	
	//Add draw layer
	this->addDrawLayer();
	
	//Create level boundaries
	this->addLevelBoundaries();
	
	//Initial variables
	cameraZoom = 0.45f;
    
	//Schedule step method
    this->schedule(schedule_selector(LineOfSight::step));
	 
    
	return this;
}

/* Set game area size */
void LineOfSight::setGameAreaSize()
{
	gameAreaSize = ccp(1000/PTM_RATIO,600/PTM_RATIO);	//Box2d units
}

void LineOfSight::step(float dt)
{
    GameArea2D::step(dt); 
	
	//Process DPad
	if(dPad->pressed){
		//[actor runWithVector:dPad.pressedVector withSpeedMod:1.0f withConstrain:NO];
		actor->body->ApplyForce(b2Vec2(dPad->pressedVector.x*25.0f, dPad->pressedVector.y*25.0f), actor->body->GetPosition());
		actor->body->SetAngularVelocity(0.0f);
		CCPoint movementVector = ccp(actor->body->GetLinearVelocity().x, actor->body->GetLinearVelocity().y);
        
		actor->body->SetTransform(actor->body->GetPosition(), -1 *GameHelper::vectorToRadians( movementVector) + PI_CONSTANT/2);
	}else{
        actor->stopRunning();
		 
	}
    
	//Follow the actor with the camera
	CCPoint actorPosition = ccp(actor->body->GetPosition().x*PTM_RATIO, actor->body->GetPosition().y*PTM_RATIO);
    this->centerCameraOnGameCoord(actorPosition); 
	
	//Make the enemies follow the actor
    this->followActorWithEnemies(); 
}

void LineOfSight::followActorWithEnemies(){
	//If enemies can see the actor they follow
	for(int i=0; i<enemies->count(); i++){
		//Align enemies
		GameActor *enemy = (GameActor*)enemies->objectAtIndex(i);
		CCPoint directionVector = CCPointMake(actor->body->GetPosition().x - enemy->body->GetPosition().x, actor->body->GetPosition().y - enemy->body->GetPosition().y);
		float radians = GameHelper::vectorToRadians(directionVector);//[GameHelper vectorToRadians:directionVector];
		enemy->body->SetTransform(enemy->body->GetPosition(), -1 * radians + PI_CONSTANT/2);
		
		RayCastClosestCallback callback;
		world->RayCast(&callback, enemy->body->GetPosition(), actor->body->GetPosition());
		
		//Did the raycast hit anything?
		enemy->setTag(   0);	//Assume we can't
		if(callback.m_hit){
			//Is the closest point the actor?
			if(callback.m_fixture->GetBody() == actor->body){
				//If so, follow the actor
				b2Vec2 normal = b2Vec2( callback.m_normal.x * -5.0f, callback.m_normal.y * -5.0f);
				enemy->body->ApplyForce(normal, actor->body->GetPosition());
				enemy->setTag(1);	//Set seeing flag to true
			}
		}
	}
}

/* Add DPad */
void LineOfSight::addDPad() {
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	 cache->addSpriteFramesWithFile("dpad_buttons.plist");
    dPad = new DPad();
	dPad->setPosition(ccp(75,75));
	dPad->setOpacity(100);
    this->addChild(dPad,5);
	 
}

/* Add actor */
void LineOfSight::addActor() {
	actor = new GameActor();
	actor->gameArea = this;
	
	actor->bodyDef->type = b2_dynamicBody;
	actor->bodyDef->position.Set(gameAreaSize.x/2,gameAreaSize.y/2);
	actor->bodyDef->userData = actor;
    
	actor->body = world->CreateBody(actor->bodyDef);
	
	actor->circleShape = new b2CircleShape();
	actor->circleShape->m_radius = 0.5f;
	actor->fixtureDef->shape = actor->circleShape;
	
	actor->body->CreateFixture(actor->fixtureDef);
	
	actor->body->SetLinearDamping(2.0f);
}

/* Add 3 enemies */
void LineOfSight::addEnemies() {
	enemies = new CCArray(3);
    
	for(int i=0; i<3; i++){
		GameActor *enemy = new GameActor();
		enemy->gameArea = this;
		enemy->setTag(0);	//We use this to store whether or not we can see the actor
		
		enemy->bodyDef->type = b2_dynamicBody;
		enemy->bodyDef->position.Set((arc4random()%((int)(gameAreaSize.x-2)))+1.0f,(arc4random()%((int)(gameAreaSize.y-2)))+1.0f);
		enemy->bodyDef->userData = enemy;
        
		enemy->body = world->CreateBody(enemy->bodyDef);
		
		enemy->circleShape = new b2CircleShape();
		enemy->circleShape->m_radius = 0.5f;
		enemy->fixtureDef->shape = enemy->circleShape;
		
		enemy->body->CreateFixture(enemy->fixtureDef);
		
        enemies->addObject(enemy);
		 
	}
}

/* Draw each enemy 'sight line' if they can see you */
void LineOfSight::drawLayer() {
	for(int i=0; i<enemies->count(); i++){
		GameActor *enemy = (GameActor*)enemies->objectAtIndex(i);
		if(enemy->getTag() == 1){
			//glColor4ub(255,255,0,32);
            
			CCPoint actorPosition = ccp(actor->body->GetPosition().x*PTM_RATIO, actor->body->GetPosition().y*PTM_RATIO);
			CCPoint enemyPosition = ccp(enemy->body->GetPosition().x*PTM_RATIO, enemy->body->GetPosition().y*PTM_RATIO);
			
			ccDrawLine(actorPosition, enemyPosition);
            
			//glColor4ub(255,255,255,255);
		}
	}
}

/* Add draw layer */
void LineOfSight::addDrawLayer() {
	_drawLayer = new DrawLayer();
	_drawLayer->setPosition( ccp(0,0)) ;
	_drawLayer->recipe = this ;
    gameNode->addChild(_drawLayer,10);
	 
}
/* DPad input */
bool LineOfSight::hudBegan(CCSet *touches,CCEvent *event)
{
    bool hudTouched = false;
    dPad->ccTouchesBegan(touches, event);
    if(dPad->pressed || touches->count() == 1){ hudTouched = true; }
	return hudTouched;
}
bool LineOfSight::hudMoved(CCSet *touches, CCEvent *event)
{
    bool hudTouched = false;
    dPad->ccTouchesMoved(touches, event);
	if(dPad->pressed || touches->count() == 1){ hudTouched = true; }
	return hudTouched;
    
}
bool LineOfSight::hudEnded(CCSet *touches, CCEvent *event)
{
    bool hudTouched = false;
    dPad->ccTouchesEnded(touches, event);
	if(dPad->pressed || touches->count() == 1){ hudTouched = true; }
	return hudTouched;
    
}


