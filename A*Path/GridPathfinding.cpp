//
//  GridPathfinding.cpp
//  A*Path
//
//  Created by Dave on 12-9-11.
//
//

#include "GridPathfinding.h"

CCLayer* GridPathfinding::runRecipe() {
	//Superclass initialization and message
    Recipe::runRecipe();
    this->showMessage("Tap and hold to draw walls.\nPress 'Find Path' to run the simulation.");

    
	//Initial variables
	gridSize = ccp(25,15);
	nodeSpace = 16.0f;
	touchedNode = ccp(0,0);
	startCoord = ccp(2,2);
	endCoord = ccp(gridSize.x-3, gridSize.y-3);
	touchedNodeIsNew = false;
	foundPath = new CCArray(3);
	addMode = true;
    
	//Seperate grid node
	gridNode = CCNode::create();
	gridNode->setPosition(ccp(35,15));
    this->addChild(gridNode,3);
 
    
	//Create 2D array (grid)
	grid = new CCArray(gridSize.x); 
	for(int x=0; x<gridSize.x; x++){
        grid->addObject( new CCArray(gridSize.y));
		//[grid addObject:[[NSMutableArray alloc] initWithCapacity:((int)gridSize.y)]];
	}
    
	//Create AStar nodes and place them in the grid
	for(int x=0; x<gridSize.x; x++){
		for(int y=0; y<gridSize.y; y++){
			//Add a node
			AStarNode *node = new  AStarNode();
			node->position = ccp(x*nodeSpace + nodeSpace/2, y*nodeSpace + nodeSpace/2);
            CCArray *tmp = (CCArray*) grid->objectAtIndex(x);
            tmp->addObject(node);
			//[[grid objectAtIndex:x] addObject:node];
		}
	}
	
	//Add neighbor nodes
	for(int x=0; x<gridSize.x; x++){
		for(int y=0; y<gridSize.y; y++){
			//Add a node
			AStarNode *node =(AStarNode*) ((CCArray*)grid->objectAtIndex(x))->objectAtIndex(y); 
            
			//Add self as neighbor to neighboring nodes
            this->addNeighborToGridNodeXY(node,x-1, y-1); //Top-Left
			this->addNeighborToGridNodeXY(node,x-1, y);//Left
			this->addNeighborToGridNodeXY(node,x-1,y+1); //Bottom-Left
			this->addNeighborToGridNodeXY(node,x ,y-1); //Top
			
			this->addNeighborToGridNodeXY(node,x ,y+1); //Bottom
			this->addNeighborToGridNodeXY(node,x+1 ,y-1); //Top-Right
			this->addNeighborToGridNodeXY(node,x+1 ,y); //Right
			this->addNeighborToGridNodeXY(node,x+1 ,y+1); //Bottom-Right
		}
	}
    
	//Add visual represenation of nodes
    this->addGridArt();
     
	/* Menu items */
    CCMenuItemFont *findPathItem = CCMenuItemFont::create("搜索路径", this,menu_selector( GridPathfinding::findPath));
    
    
	//CCMenuItem *findPathItem = CCMenuItemFont::create("Find Path",this,menu_selector(GridPathfinding::foundPath));
    //[CCMenuItemFont itemFromString:@"Find Path" target:self selector:@selector(findPath:)];
	findPathItem->setScale(0.65f);
    CCMenuItemToggle *switchModeItem = CCMenuItemToggle::createWithTarget(this, menu_selector(GridPathfinding::switchMode),CCMenuItemFont::create("去除墙"),CCMenuItemFont::create("添加墙") );
    
    
	//CCMenuItemToggle *switchModeItem =CCMenuItemToggle::create(this,menu_selector(GridPathfinding::switchMode) ,CCMenuItemFont::create("Switch Mode: Remove Wall"),CCMenuItemFont::create("Switch Mode: Add Wall"),NULL);
	switchModeItem->setScale(0.65f);
   
	CCMenu *menu =  CCMenu::create(findPathItem,switchModeItem);//[CCMenu menuWithItems:findPathItem, switchModeItem, nil];
	//[menu alignItemsVertically];
    menu->alignItemsVertically();
	menu->setPosition(ccp(350,290));
    this->addChild(menu,5);
    
	//Add draw layer
	this->addDrawLayer();
	//Schedule step method
    this->schedule(schedule_selector(GridPathfinding::step));
	 
    
	return this;
}

void GridPathfinding:: step(float delta ){
	//Flip a touched node
	if(touchedNodeIsNew){
        this->flipNodeWithTouchedNode(touchedNode);
		touchedNodeIsNew = false;
	}
}

/* Switch between adding nodes and adding walls */
void GridPathfinding::switchMode(CCObject *sender) {
	addMode = !addMode;
}

/* Add draw layer */
void  GridPathfinding::addDrawLayer() {
	_drawLayer = new DrawLayer();
	_drawLayer->setPosition (ccp(0,0));
	_drawLayer->recipe = this;
    gridNode->addChild(_drawLayer,10);
}

void GridPathfinding:: drawLayer() {
	//If we have less than two nodes in our path we return
    if (!foundPath) {
        return;
    }
	if(foundPath->count() < 2){
		return;
	}
    
	//glColor4ub(0,0,255,255);
	
	//Draw all edges of the path
	for(int i=0; i<foundPath->count()-1; i++){
        CCPoint p1 = CCValue::ccpointFromCCValue((CCValue*)foundPath->objectAtIndex(i));
	    CCPoint p2 = CCValue::ccpointFromCCValue((CCValue*)foundPath->objectAtIndex(i+1));
		ccDrawLine(p1,p2);
	}
    
	//glColor4ub(255,255,255,255);
}

/* Find a path from the startNode to the endNode */
void GridPathfinding::findPath(CCObject *sender)
{
    
	AStarNode *startNode =(AStarNode*) ((CCArray*)grid->objectAtIndex(startCoord.x))->objectAtIndex( startCoord.y);
    
    
	AStarNode *endNode = (AStarNode*) ((CCArray*)grid->objectAtIndex(endCoord.x))->objectAtIndex( endCoord.y);
    
	if(foundPath){
        foundPath->removeAllObjects();
        foundPath->release();
	}
	foundPath = NULL;
	
	//Run the pathfinding algorithm
	foundPath = AStarPathNode::findPathFromTo(startNode, endNode); // [AStarPathNode findPathFrom:startNode to:endNode];
	
	if(!foundPath){
        this->showMessage("No Path Found");
		 
	}else{
        this->showMessage("Found Path");
	 
	}
}

/* Helper method for adding neighbor nodes */
void GridPathfinding::addNeighborToGridNodeXY(AStarNode *node, int x, int y) {
	if(x >= 0 && y >= 0 && x < gridSize.x && y < gridSize.y){
        
		AStarNode *neighbor =(AStarNode*) ((CCArray*)grid->objectAtIndex(x))->objectAtIndex(y);
        node->neighbors->addObject(neighbor);
		 
	}
}

/* Add sprites which correspond to grid nodes */
void GridPathfinding::addGridArt() {
	sprites = new CCDictionary();
    
	for(int x=0; x<gridSize.x; x++){
		for(int y=0; y<gridSize.y; y++){
			AStarNode *node = (AStarNode*)((CCArray*)grid->objectAtIndex(x))->objectAtIndex(y) ;
            
			CCSprite *sprite = CCSprite::create("gridNode.png" );
            sprite->setPosition(node->position);
			 
			if(node->active){
				sprite->setColor(ccc3(200,200,200) );
			}else{
                sprite->setColor(ccc3(100,100,100) );
			}
            gridNode->addChild(sprite);
            CCString *s = CCString::createWithFormat("(%i,%i)",x,y);
            sprites->setObject(sprite,s->getCString());
			//[sprites setObject:sprite forKey:[NSString stringWithFormat:@"(%i,%i)",x,y]];
		}
	}
	
	//Add start point at (0,0)
	CCSprite *startSprite = CCSprite::create("start_button.png");
	startSprite->setPosition( ccp(startCoord.x*nodeSpace + nodeSpace/2, startCoord.y*nodeSpace + nodeSpace/2)) ;
	gridNode->addChild(startSprite);
	//Add end point at (gridSize.x-1,gridSize.y-1)
	CCSprite *endSprite = CCSprite::create("end_button.png");
	endSprite->setPosition( ccp(endCoord.x*nodeSpace + nodeSpace/2,endCoord.y*nodeSpace + nodeSpace/2));
    gridNode->addChild(endSprite);
	 
}

/* Process touching and moving to add/remove walls */
void GridPathfinding::ccTouchesBegan(CCSet *touches,CCEvent *event)
{
    CCTouch *touch = (CCTouch*)touches->anyObject();
	CCPoint point = touch->getLocationInView(); 
	point = CCDirector::sharedDirector()->convertToGL( point);
	
	int x = ((point.x - gridNode->getPosition().x) / nodeSpace);
	int y = ((point.y - gridNode->getPosition().y) / nodeSpace);
	CCPoint tp = CCPointMake(x,y);
	
	touchedNodeIsNew = true;
	touchedNode = tp;
}

void GridPathfinding::ccTouchesMoved(CCSet *touches ,CCEvent *event)
{
	CCTouch *touch = (CCTouch*)touches->anyObject();
	CCPoint point = touch->getLocationInView(); 
	point = CCDirector::sharedDirector()->convertToGL( point);
	
	int x = ((point.x - gridNode->getPosition().x) / nodeSpace);
	int y = ((point.y - gridNode->getPosition().y) / nodeSpace);
	CCPoint tp = CCPointMake(x,y);
	
	if(tp.x != touchedNode.x || tp.y != touchedNode.y){
		touchedNodeIsNew = true;
	}
	touchedNode = tp;
}
/* Flip a node when touched */
void GridPathfinding::flipNodeWithTouchedNode(CCPoint point)
{
    int x = point.x;
	int y = point.y;
	
	if(x == 0 && y == 0){
		return;
	}
	if(x == gridSize.x-1 && y == gridSize.y-1){
		return;
	}
	
	if(x < 0 || y < 0 || x > gridSize.x-1 || y > gridSize.y-1){
		return;
	}
	
	AStarNode *node = (AStarNode*)((CCArray*)grid->objectAtIndex(x))->objectAtIndex(y);
    
	CCSprite *sprite = (CCSprite*)sprites->objectForKey( CCString::createWithFormat("(%i,%i)",x,y)->getCString());
	
	if(node->active && addMode){
		//Remove node as neighbor and vice versa
		node->active = false;
        sprite->setColor(ccc3(100,100,100));
		 
	}else if(!addMode){
		node->active = true;
		//Change sprite color
		sprite->setColor(ccc3(200,200,200));
	}
}

void GridPathfinding::cleanRecipe()
{
//    sprites->release();
//    foundPath->release();
//    foundPath = NULL;
    Recipe::cleanRecipe();
	 
}