//
//  AStarNode.cpp
//  A*Path
//
//  Created by Dave on 12-9-11.
//
//

#include "AStarNode.h"

AStarNode::AStarNode():CCObject() {

    active = true;
    neighbors = new CCArray(4);
    costMultiplier = 1.0f;
}

void  AStarNode::release(){
    neighbors->removeAllObjects();
    neighbors->release();
    CCObject::release();

}
/* Cost to node heuristic */
float AStarNode::costToNode(AStarNode* node)
{
	CCPoint src = ccp(this->position.x, this->position.y);
	CCPoint dst = ccp(node->position.x, node->position.y);
	float cost = GameHelper::distanceP1toP2(src, dst);  // [GameHelper distanceP1:src toP2:dst] * node.costMultiplier;
	return cost;
}

/* Helper method: Is a node in a given list? */
bool AStarNode::isNodeInList(AStarNode* a,CCArray* list) {
	for(int i=0; i<list->count(); i++){
		AStarNode *b = (AStarNode*)list->objectAtIndex(i);
		if(a->position.x == b->position.x && a->position.y == b->position.y){
			return true;
		}
	}
	return false;
}
