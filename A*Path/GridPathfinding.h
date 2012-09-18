//
//  GridPathfinding.h
//  A*Path
//
//  Created by Dave on 12-9-11.
//
//

#ifndef __A_Path__GridPathfinding__
#define __A_Path__GridPathfinding__
#include "Recipe.h"
#include "AStarNode.h"
#include "AStarPathNode.h"
#include "DrawLayer.h"
#include "CCValue.h"

class  GridPathfinding : public Recipe{
public:
    CCArray *grid;
	CCPoint gridSize;
	float nodeSpace;	//The space between each node
	CCNode *gridNode;	//Where we draw everything
	CCDictionary *sprites;	//A way to keep track of our sprites
	CCPoint touchedNode;	//Where we touched
	bool touchedNodeIsNew;
	CCArray *foundPath;
	DrawLayer *_drawLayer;
	CCPoint startCoord;
	CCPoint endCoord;
	bool addMode;
    
    
    
    virtual CCLayer* runRecipe();
    virtual void step(float delta);
    virtual void addDrawLayer();
    virtual void drawLayer();
    virtual void switchMode(CCObject* sender);
    virtual void findPath(CCObject* sender);
    virtual void addNeighborToGridNodeXY(AStarNode* node,int x,int y);
    virtual void addGridArt();
    virtual void ccTouchesBegan(CCSet *touches ,CCEvent *event);
    virtual void ccTouchesMoved(CCSet *touches ,CCEvent *event);
    virtual void flipNodeWithTouchedNode(CCPoint point);
    virtual void cleanRecipe();
}; 


#endif /* defined(__A_Path__GridPathfinding__) */
