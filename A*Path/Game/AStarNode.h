//
//  AStarNode.h
//  A*Path
//
//  Created by Dave on 12-9-11.
//
//

#ifndef __A_Path__AStarNode__
#define __A_Path__AStarNode__

#include "Base.h"
#include "GameHelper.h"
class AStarNode : public CCObject
{
public:
    AStarNode();
    CCPoint position;	//The node's position on our map
	CCArray *neighbors;	//An array of neighbor AStarNode objects
	bool active;	//Is this node active?
	float costMultiplier;	//Use this to multiply the normal cost to reach this node.
    virtual float costToNode(AStarNode* node);
    static bool isNodeInList(AStarNode* a,CCArray* list);
 
  
};
#endif /* defined(__A_Path__AStarNode__) */
