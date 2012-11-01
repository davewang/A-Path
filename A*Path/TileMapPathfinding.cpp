//
//  TileMapPathfinding.cpp
//  A*Path
//
//  Created by Dave on 12-10-15.
//
//

#include "TileMapPathfinding.h"

CCLayer*  TileMapPathfinding::runRecipe()
{
	//Superclass initialization and message
    Tiled::runRecipe();
    message->setString("Tap the screen move the actor to that point.\nTap again to queue another waypoint.");
	 
    
	//Shorter variable names
	float mw = tileMap->getMapSize().width;
	float mh = tileMap->getMapSize().height;
	float tw = tileMap->getTileSize().width;
	float th = tileMap->getTileSize().height;
    
	actorRadius = actor->circleShape->m_radius;
    
	//Create 2D array (grid)
	grid = CCArray::createWithCapacity((int)mw);//[[NSMutableArray alloc] initWithCapacity:((int)mw)];
    grid->retain();
	for(int x=0; x<mw; x++){
        grid->addObject(CCArray::createWithCapacity((int)mh));
	}
   
	//Create active and inactive nodes determined by the "Collidable" TMX layer
	CCTMXLayer *collidableLayer =  tileMap->layerNamed("Collidable");
	for(int x=0; x<mw; x++){
		for(int y=0; y<mh; y++){
			//Add a node
			AStarNode *node = new  AStarNode();
			node->position = ccp(x*tw + tw/2, y*th + th/2);
             
			if(collidableLayer!=NULL&&collidableLayer->tileAt(ccp(x,y))){
                node->active = false; }
            ((CCArray*)grid->objectAtIndex(x))->addObject(node);
			 
		}
	}
	
	//Add neighbors
	for(int x=0; x<mw; x++){
		for(int y=0; y<mh; y++){
			//Add a node
			AStarNode *node = (AStarNode*)((CCArray*)grid->objectAtIndex(x))->objectAtIndex(y);//[[grid objectAtIndex:x] objectAtIndex:y];
            
			//Add self as neighbor to neighboring nodes
            
			//[self addNeighbor:node toGridNodeX:x-1 Y:y-1];
            this->addNeighborToGridNodeXY(node, x-1, y-1);//Top-Left
			//[self addNeighbor:node toGridNodeX:x-1 Y:y];
            this->addNeighborToGridNodeXY(node, x-1, y);//Left
			//[self addNeighbor:node toGridNodeX:x-1 Y:y+1];
            this->addNeighborToGridNodeXY(node, x-1, y+1);//Bottom-Left
			//[self addNeighbor:node toGridNodeX:x Y:y-1];
            this->addNeighborToGridNodeXY(node, x, y-1);//Top
			
			//[self addNeighbor:node toGridNodeX:x Y:y+1];
            this->addNeighborToGridNodeXY(node, x, y+1);//Bottom
			//[self addNeighbor:node toGridNodeX:x+1 Y:y-1];
            this->addNeighborToGridNodeXY(node, x+1, y-1);//Top-Right
			//[self addNeighbor:node toGridNodeX:x+1 Y:y];
            this->addNeighborToGridNodeXY(node, x+1, y);//Right
			//[self addNeighbor:node toGridNodeX:x+1 Y:y+1];
            this->addNeighborToGridNodeXY(node, x+1, y+1);//Bottom-Right
		}
	}
    
	//Remove neighbors from positive RayCast tests
	for(int x=0; x<mw; x++){
		for(int y=0; y<mh; y++){
			//Add a node
			AStarNode *node = (AStarNode*) ((CCArray*)grid->objectAtIndex(x))->objectAtIndex(y);//[[grid objectAtIndex:x] objectAtIndex:y];
			
			//If a node itself is colliding with an object we cut off all connections
			for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()){
				if (b->GetUserData() != NULL) {
					GameObject *obj = (GameObject*)b->GetUserData();
					if(obj->polygonShape){
						b2Vec2 nodePosition = b2Vec2(node->position.x/PTM_RATIO, node->position.y/PTM_RATIO);
						
						//Test this node point against this polygon
						if(obj!=NULL&&obj->polygonShape->TestPoint(b->GetTransform(),nodePosition)){
							for(int i=0; i<node->neighbors->count(); i++){
								//Remove connections
                                 
								AStarNode *neighbor = (AStarNode*) node->neighbors->objectAtIndex(i);
                                node->neighbors->removeObject(neighbor);
                                neighbor->neighbors->removeObject(node);
								 
							}
						}
					}
				}
			}
            
			//Test all node to neighbor connections using a RayCast test
			for(int i=0; i<node->neighbors->count(); i++){
				AStarNode *neighbor = (AStarNode*)node->neighbors->objectAtIndex(i);// [node.neighbors objectAtIndex:i];
				
				//Do a RayCast from the node to the neighbor.
				//If there is something in the way, remove the link
				b2Vec2 nodeP = b2Vec2(node->position.x/PTM_RATIO, node->position.y/PTM_RATIO);
				b2Vec2 neighborP = b2Vec2(neighbor->position.x/PTM_RATIO, neighbor->position.y/PTM_RATIO);
				
				//Do 4 tests (based on actor size)
				for(float x = -actorRadius; x <= actorRadius; x+= actorRadius*2){
					for(float y = -actorRadius; y <= actorRadius; y+= actorRadius*2){
						RayCastAnyCallback callback;
						world->RayCast(&callback, b2Vec2(nodeP.x+x,nodeP.y+y), b2Vec2(neighborP.x+x,neighborP.y+y));
                        
						if(callback.m_hit){
							//Remove connections
                            node->neighbors->removeObject(neighbor);
                            neighbor->neighbors->removeObject(node);
					 
							break; break;
						}
					}
				}
			}
		}
	}
    
	//Initial variables
	dPad->setVisible(true);
	cameraZoom = 0.75f;
	actor->runSpeed = 5.0f;
    
	//Add draw layer
    this->addDrawLayer();
	
	return this;
}

void TileMapPathfinding::step(float dt)
{
	//DPad disabled
	dPad->pressed = true;
    
    Tiled::step(dt);
	
	//Process actor waypoints
    actor->processWaypoints();
	
	//Orient the actor properly
	if(actor->waypoints->count() > 0){
		CCPoint movementVector = ccp(actor->body->GetLinearVelocity().x, actor->body->GetLinearVelocity().y);
       
		actor->body->SetTransform(actor->body->GetPosition(), -1 *  GameHelper::vectorToRadians(movementVector) + PI_CONSTANT/2);
	}
    
    //Follow the actor with the camera
	CCPoint actorPosition = ccp(actor->body->GetPosition().x*PTM_RATIO, actor->body->GetPosition().y*PTM_RATIO);
    this->centerCameraOnGameCoord(actorPosition);
	
}

/* Disable camera following */
void  TileMapPathfinding::cameraFollowActor() {
	//Do nothing
}

/* Enable camera zooming */
void TileMapPathfinding::setCameraZoom(float zoom) {
	if(zoom < 0.1f){ zoom = 0.1f; }	//Lower limit
	if(zoom > 4.0f){ zoom = 4.0f; }	//Upper limit
	gameNode->setScale(zoom);
	cameraZoom = zoom;
}

/* Add neighbor helper method */
void TileMapPathfinding:: addNeighborToGridNodeXY(AStarNode *node, int x, int y)
{
	float mw = tileMap->getMapSize().width;
	float mh = tileMap->getMapSize().height;
	if(x >= 0 && y >= 0 && x < mw && y < mh){
        
		AStarNode *neighbor = (AStarNode*) ((CCArray*)grid->objectAtIndex(x))->objectAtIndex(y);
        node->neighbors->addObject(neighbor); 
	}
}

/* Add draw layer */
void TileMapPathfinding::addDrawLayer() {
    
    
	_drawLayer = new DrawLayer();
	_drawLayer->setPosition(ccp(0,0));
	_drawLayer->recipe = this;
    gameNode->addChild(_drawLayer,10000);
	 
}

/* Draw layer */
void TileMapPathfinding::drawLayer() {
	//Draw waypoints
	//glColor4ub(255,255,0,32);
	CCPoint actorPosition = ccp(actor->body->GetPosition().x*PTM_RATIO, actor->body->GetPosition().y*PTM_RATIO);
	if(actor->waypoints->count() == 1){
		GameWaypoint *gw = (GameWaypoint*) actor->waypoints->objectAtIndex(0);
          
		ccDrawLine(actorPosition,gw->position);
	}else if(actor->waypoints->count() > 1){
		for(int i=0; i<actor->waypoints->count()-1; i++){
            
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
	//glColor4ub(255,255,255,255);
}

/* Find a path and add it (as a set of waypoints) when we tap the screen */
void TileMapPathfinding::tapWithPoint(CCPoint point) {
	//Convert touch coordinate to physical coordinate
	CCPoint endPoint =  this->convertTouchCoord(point);//[self convertTouchCoord:point];
	if(endPoint.x < 0 || endPoint.y < 0 || endPoint.x >= gameAreaSize.x*PTM_RATIO || endPoint.y >= gameAreaSize.y*PTM_RATIO){
		return;
	}
	//Actor position
	CCPoint actorPosition = ccp(actor->body->GetPosition().x*PTM_RATIO, actor->body->GetPosition().y*PTM_RATIO);
	
	//We use the last waypoint position if applicable
	if(actor->waypoints->count() > 0){
        
		actorPosition = ((GameWaypoint*)actor->waypoints->objectAtIndex(actor->waypoints->count()-1))->position;
	}
	
	float tw = tileMap->getTileSize().width;
	float th = tileMap->getTileSize().height;
	float mw = tileMap->getMapSize().width;
	float mh = tileMap->getMapSize().height;
	
    
	//Starting node
	AStarNode *startNode = (AStarNode*)((CCArray*)grid->objectAtIndex((int)(actorPosition.x/tw)))->objectAtIndex((int)(actorPosition.y/th));
    
	//Make sure the start node is actually properly connected
	if(startNode->neighbors->count() == 0){
		bool found = false; float n = 1;
		while(!found){
			//Search the nodes around this point for a properly connected starting node
			for(float x = -n; x<= n; x+= n){
				for(float y = -n; y<= n; y+= n){
					if(x == 0 && y == 0){ continue; }
					float xIndex = ((int)(actorPosition.x/tw))+x;
					float yIndex = ((int)(actorPosition.y/th))+y;
					if(xIndex >= 0 && yIndex >= 0 && xIndex < mw && yIndex < mh){
                       
						AStarNode *node =  (AStarNode*)((CCArray*)grid->objectAtIndex(xIndex))->objectAtIndex(yIndex);
						if(node->neighbors->count() > 0){
							startNode = node;
							found = true;
							break; break;
						}
					}
				}
			}
			n += 1;
		}
	}
	
	//End node
	AStarNode *endNode = (AStarNode*)((CCArray*)grid->objectAtIndex((int)(endPoint.x/tw)))->objectAtIndex((int)(endPoint.y/th));
    
	//Run the pathfinding algorithm
	CCArray *foundPath = AStarPathNode::findPathFromTo(startNode, endNode); //[AStarPathNode findPathFrom:startNode to:endNode];
	
	if(!foundPath){
        this->showMessage("No Path Found");
		//[self showMessage:@"No Path Found"];
	}else{
        this->showMessage("Found Path");
		//[self showMessage:@"Found Path"];
		//Add found path as a waypoint set to the actor
		for(int i=foundPath->count()-1; i>=0; i--){
            
           
			CCPoint pathPoint = CCValue::ccpointFromCCValue( ((CCValue*)foundPath->objectAtIndex(i)) );
            actor->addWaypoint(GameWaypoint::createWithPositionWithSpeedMod(pathPoint, 1.0f)); 
		}
	}
    if (foundPath!=NULL ) {
        foundPath->release();
        foundPath=NULL;
    }
	
    
}

/* DPad disabled */
bool TileMapPathfinding::hudBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
    bool hudTouched = false;
    dPad->ccTouchesBegan(touches, event);
    if(dPad->pressed || touches->count() == 1){ hudTouched = true; }
	return hudTouched;
    //return false;
}
bool TileMapPathfinding::hudMoved(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
    bool hudTouched = false;
    dPad->ccTouchesMoved(touches, event);
	if(dPad->pressed || touches->count() == 1){ hudTouched = true; }
	return hudTouched;
    //return false;
}
bool TileMapPathfinding::hudEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
    bool hudTouched = false;
    dPad->ccTouchesEnded(touches, event);
	if(dPad->pressed || touches->count() == 1){ hudTouched = true; }
	return hudTouched;
    //return false;
}
  
