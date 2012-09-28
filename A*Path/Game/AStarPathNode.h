//
//  AStarPathNode.h
//  A*Path
//
//  Created by Dave on 12-9-11.
//
//

#ifndef __A_Path__AStarPathNode__
#define __A_Path__AStarPathNode__
 
#include "GameHelper.h"
#include "AStarNode.h"
#include "CCValue.h"
class AStarPathNode :public  CCObject
{
public:
    AStarNode *node;	//The actual node this "path" node points to
	AStarPathNode *previous;	//The previous node on our path
	float cost;	//The cumulative cost of reaching this node
    
    AStarPathNode();
    static AStarPathNode* createWithAStarNode(AStarNode* node);
    static CCArray* findPathFromTo(AStarNode* fromNode,AStarNode* toNode);
    static AStarPathNode* lowestCostNodeInArray(CCArray *a);
    static bool isPathNodeinList(AStarPathNode* a,CCArray* list);
    virtual void release();

};

#endif /* defined(__A_Path__AStarPathNode__) */
