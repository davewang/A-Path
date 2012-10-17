//
//  Box2DPathfinding.cpp
//  A*Path
//
//  Created by Dave on 12-10-13.
//
//

#include "Box2DPathfinding.h"
 
CCLayer* Box2DPathfinding::runRecipe()
{
	//Set game area size
	gameAreaSize = ccp(1000/PTM_RATIO,600/PTM_RATIO);	//Box2d units
    
	//Superclass initialization and message
	GameArea2D::runRecipe();
    message->setString("Tap the screen move the actor to that point.\nTap again to queue another waypoint." );
	
    
	//Add some geometry
    this->addRandomPolygons(5);
    this->addRandomBoxes(5);
    
	//Initial variables
	nodeSpace = 32.0f;
	cameraZoom = 0.45f;
	actorRadius = nodeSpace/PTM_RATIO/3;
	
	//Add draw layer
    this->addDrawLayer();
	
	//Create level boundaries
    this->addLevelBoundaries();
    
	//Create 2D array (grid)
	gridSizeX = (int)(gameAreaSize.x*PTM_RATIO/nodeSpace);
	gridSizeY = (int)(gameAreaSize.y*PTM_RATIO/nodeSpace);
	grid = new CCArray(gridSizeX);
    //grid->init();
    //CCArray::createWithCapacity(gridSizeX);
	for(int x=0; x<gridSizeX; x++){
        
        grid->addObject(CCArray::createWithCapacity(gridSizeY) );
        //grid->addObject(new CCArray(gridSizeY));
    }
    
	//Create AStar nodes
	for(int x=0; x<gridSizeX; x++){
		for(int y=0; y<gridSizeY; y++){
			//Add a node
			AStarNode *node = new AStarNode();
            node->position = ccp(x*nodeSpace + nodeSpace/2, y*nodeSpace + nodeSpace/2);
            ((CCArray*)grid->objectAtIndex(x))->addObject(node);
			
		}
	}
    
	//Add neighbors
	for(int x=0; x<gridSizeX; x++){
		for(int y=0; y<gridSizeY; y++){
			//Add a node
			AStarNode *node = (AStarNode*)((CCArray*)grid->objectAtIndex(x))->objectAtIndex(y);// [[grid objectAtIndex:x] objectAtIndex:y];
            
			//Add self as neighbor to neighboring nodes
			//[self addNeighbor:node toGridNodeX:x-1 Y:y-1];
            this->addNeighborToGridNodeXY(node, x-1, y-1); //Top-Left
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
	
	//Remove neighbors from positive TestPoint and RayCast tests
	for(int x=0; x<gridSizeX; x++){
		for(int y=0; y<gridSizeY; y++){
			//Add a node
			AStarNode *node = (AStarNode*)((CCArray*)grid->objectAtIndex(x))->objectAtIndex(y);//[[grid objectAtIndex:x] objectAtIndex:y];
			
			//If a node itself is colliding with an object we cut off all connections
			for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()){
				if (b->GetUserData() != NULL) {
					GameObject *obj = (GameObject*)b->GetUserData();
					if(obj->polygonShape){
						b2Vec2 nodePosition = b2Vec2(node->position.x/PTM_RATIO, node->position.y/PTM_RATIO);
						
						//Test this node point against this polygon
						if(obj->polygonShape->TestPoint(b->GetTransform(), nodePosition)){
							for(int i=0; i<node->neighbors->count(); i++){
								//Remove connections
								AStarNode *neighbor = (AStarNode*)node->neighbors->objectAtIndex(i);//[node.neighbors objectAtIndex:i];
                                node->neighbors->removeObject(neighbor);
								//[node.neighbors removeObject:neighbor];
                                neighbor->neighbors->removeObject(node);
								//[neighbor.neighbors removeObject:node];
							}
						}
					}
				}
			}
			
			//Test all node to neighbor connections using a RayCast test
			for(int i=0; i<node->neighbors->count(); i++){
				AStarNode *neighbor = (AStarNode*)node->neighbors->objectAtIndex(i);//[node.neighbors objectAtIndex:i];
				
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
							//[node.neighbors removeObject:neighbor];
							//[neighbor.neighbors removeObject:node];
							break; break;
						}
					}
				}
			}
		}
	}
	
	//Create Actor
    this->addActor();
	//[self addActor];
    
	//Schedule step method
    this->schedule(schedule_selector(Box2DPathfinding::step));
	//[self schedule:@selector(step:)];
    
	return this;
}

/* Add neighbor helper method */
void Box2DPathfinding::addNeighborToGridNodeXY(AStarNode *node, int x, int y)
{
	if(x >= 0 && y >= 0 && x < gridSizeX && y < gridSizeY){
		AStarNode *neighbor = (AStarNode*)((CCArray*)grid->objectAtIndex(x))->objectAtIndex(y); 
        //[[grid objectAtIndex:x] objectAtIndex:y];
		//if(![AStarNode isNode:neighbor inList:node.neighbors]){
        if (!AStarNode::isNodeInList(neighbor, node->neighbors)) {
            node->neighbors->addObject(neighbor);
        }
		 
	}
}

void Box2DPathfinding::step(float dt)
{
    GameArea2D::step(dt);
	
	//Process actor waypoints
    actor->processWaypoints(); 
	
	//Turn actor to face the proper direction
	if(actor->waypoints->count() > 0){
		CCPoint movementVector = ccp(actor->body->GetLinearVelocity().x, actor->body->GetLinearVelocity().y);
        
		actor->body->SetTransform(actor->body->GetPosition(), -1 * GameHelper::vectorToRadians(movementVector)+ PI_CONSTANT/2);
	}
}

/* Add actor */
void Box2DPathfinding::addActor()
{
	actor = new GameActor();//[[GameActor alloc] init];
	actor->gameArea = this;
	
	actor->bodyDef->type = b2_dynamicBody;
	actor->bodyDef->position.Set(240/PTM_RATIO,160/PTM_RATIO);
	actor->bodyDef->userData = actor;
    
	actor->body = world->CreateBody(actor->bodyDef);
	
	actor->circleShape = new b2CircleShape();
	actor->circleShape->m_radius = actorRadius;
	actor->fixtureDef->shape = actor->circleShape;
	actor->fixtureDef->isSensor = false;
	
	actor->body->CreateFixture(actor->fixtureDef);
}

/* Draw layer */
void Box2DPathfinding::drawLayer()
{
	//Draw waypoints
  	glColorMask(255,255,255,0.2);
	CCPoint actorPosition = ccp(actor->body->GetPosition().x*PTM_RATIO, actor->body->GetPosition().y*PTM_RATIO);
	if(actor->waypoints->count() == 1){
		GameWaypoint *gw = (GameWaypoint*)actor->waypoints->objectAtIndex(0);// [actor.waypoints objectAtIndex:0];
		ccDrawLine(actorPosition, gw->position);
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
	//glColorMask(255,255,255,255);
	
	//Only draw the AStarNodes and connections in the simulator (too slow for the device)
	//CCString *model = CCString::create("iPhone Simulator");//[[UIDevice currentDevice] model];
    //model->isEqual(CCString::create("iPhone Simulator"));
	//if([model isEqualToString:@"iPhone Simulator"] || [model isEqualToString:@"iPad Simulator"])
    {
		//Draw AStarNodes
	//	glColorMask(16,16,16,8);
		for(int x=0; x<gridSizeX; x++){
			for(int y=0; y<gridSizeY; y++){
				//Draw node
				AStarNode *node = (AStarNode*)((CCArray*)grid->objectAtIndex(x))->objectAtIndex(y);;//[[grid objectAtIndex:x] objectAtIndex:y];
				ccDrawPoint(node->position);
				
				//Draw neighbor lines (there is going to be a lot of them)
				for(int i=0; i<node->neighbors->count(); i++){
					AStarNode *neighbor = (AStarNode*)node->neighbors->objectAtIndex(i);
					ccDrawLine(node->position, neighbor->position);
				}
			}
		}
	//	glColorMask(255,255,255,255);
	}
}

/* Add draw layer */
void Box2DPathfinding::addDrawLayer()
{
	_drawLayer = new DrawLayer(); //[[DrawLayer alloc] init];
	_drawLayer->setPosition(ccp(0,0));
	_drawLayer->recipe = this;
    gameNode->addChild(_drawLayer,10);
    
}

/* Find a path and add it (as a set of waypoints) when we tap the screen */
void Box2DPathfinding::tapWithPoint(CCPoint point)
{
	//Convert touch coordinate to physical coordinate
	CCPoint endPoint = this->convertTouchCoord(point);
    
    // [self convertTouchCoord:point];
	if(endPoint.x < 0 || endPoint.y < 0 || endPoint.x >= gameAreaSize.x*PTM_RATIO || endPoint.y >= gameAreaSize.y*PTM_RATIO){
		return;
	}
	
	//Actor position
	CCPoint actorPosition = ccp(actor->body->GetPosition().x*PTM_RATIO, actor->body->GetPosition().y*PTM_RATIO);
	
	//We use the last waypoint position if applicable
	if(actor->waypoints->count() > 0){
		actorPosition = ((GameWaypoint*)actor->waypoints->objectAtIndex(actor->waypoints->count()-1))->position;//[[actor.waypoints objectAtIndex:actor.waypoints.count-1] position];
	}
	
	//Starting node
	AStarNode *startNode = (AStarNode*)((CCArray*)grid->objectAtIndex(actorPosition.x/nodeSpace))->objectAtIndex(actorPosition.y/nodeSpace);
    //[[grid objectAtIndex:(int)(actorPosition.x/nodeSpace)] objectAtIndex:(int)(actorPosition.y/nodeSpace)];
    
	//Make sure the start node is actually properly connected
	if(startNode->neighbors->count() == 0){
		bool found = false; float n = 1;
		while(!found){
			//Search the nodes around this point for a properly connected starting node
			for(float x = -n; x<= n; x+= n){
				for(float y = -n; y<= n; y+= n){
					if(x == 0 && y == 0){ continue; }
					float xIndex = ((int)(actorPosition.x/nodeSpace))+x;
					float yIndex = ((int)(actorPosition.y/nodeSpace))+y;
					if(xIndex >= 0 && yIndex >= 0 && xIndex < gridSizeX && yIndex < gridSizeY){
						AStarNode *node = (AStarNode*)((CCArray*)grid->objectAtIndex(xIndex))->objectAtIndex(yIndex);
                        
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
	AStarNode *endNode = (AStarNode*)((CCArray*)grid->objectAtIndex(endPoint.x/nodeSpace))->objectAtIndex(endPoint.y/nodeSpace);
   // [[grid objectAtIndex:(int)(endPoint.x/nodeSpace)] objectAtIndex:(int)(endPoint.y/nodeSpace)];
    
	//Run the pathfinding algorithm
	CCArray *foundPath = AStarPathNode::findPathFromTo(startNode, endNode);//[AStarPathNode findPathFrom:startNode to:endNode];
	
	if(!foundPath){
        this->showMessage("No Path Found");
	}else{
        this->showMessage("Found Path");
		//Add found path as a waypoint set to the actor
		for(int i=foundPath->count()-1; i>=0; i--){
            
			CCPoint pathPoint = CCValue::ccpointFromCCValue((CCValue*)foundPath->objectAtIndex(i));//[[foundPath objectAtIndex:i] CGPointValue];
            actor->addWaypoint(GameWaypoint::createWithPositionWithSpeedMod(pathPoint, 1.0f))
            ;
		 
		}
	}
    if (foundPath) {
        foundPath->release();
        foundPath = NULL;
    }
	
}
