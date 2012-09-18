//
//  AStarPathNode.cpp
//  A*Path
//
//  Created by Dave on 12-9-11.
//
//

#include "AStarPathNode.h"

AStarPathNode::AStarPathNode() {
        CCObject::CCObject();
    	cost = 0.0f;
}

AStarPathNode* AStarPathNode::createWithAStarNode(AStarNode* node) {
	if(!node){	//Can't have a path node without a real node
		return NULL;
	}
	AStarPathNode *pathNode = new AStarPathNode();
	pathNode->node = node;
    pathNode->autorelease();
	return pathNode;
}

/* Our implementation of the A* search algorithm */
CCArray*  AStarPathNode::findPathFromTo(AStarNode *fromNode, AStarNode *toNode) {
	CCArray *foundPath = CCArray::createWithCapacity(2);
    foundPath->retain();
     
	if(fromNode->position.x == toNode->position.x && fromNode->position.y == toNode->position.y){
		return NULL;
    }
	
	CCArray *openList = CCArray::create();
	CCArray *closedList = CCArray::create();

	
	AStarPathNode *currentNode = NULL;
	AStarPathNode *aNode = NULL;
    
	AStarPathNode *startNode = AStarPathNode::createWithAStarNode(fromNode);
	AStarPathNode *endNode = AStarPathNode::createWithAStarNode(toNode);
	openList->addObject(startNode);
	while(openList->count() > 0){
		currentNode = AStarPathNode::lowestCostNodeInArray(openList);
        
		if( currentNode->node->position.x == endNode->node->position.x &&
           currentNode->node->position.y == endNode->node->position.y){
			
			//Path Found!
			aNode = currentNode;
			while(aNode!=NULL&&aNode->previous != NULL){
				//Mark path
                foundPath->addObject(CCValue::valueWithCCPoint(ccp(aNode->node->position.x, aNode->node->position.y)));
				aNode = aNode->previous;
			}
            foundPath->addObject(CCValue::valueWithCCPoint(ccp(aNode->node->position.x, aNode->node->position.y)));
			return foundPath;
		}else{
			//Still searching
            closedList->addObject(currentNode);
            openList->removeObject(currentNode);
			 
			
			for(int i=0; i<currentNode->node->neighbors->count(); i++){
				AStarPathNode *aNode = AStarPathNode::createWithAStarNode((AStarNode*)currentNode->node->neighbors->objectAtIndex(i));
				aNode->cost = currentNode->cost+currentNode->node->costToNode(aNode->node)+aNode->node->costToNode(endNode->node);
				aNode->previous = currentNode;
				if(aNode->node->active && ! AStarPathNode::isPathNodeinList(aNode, openList) && !AStarPathNode::isPathNodeinList(aNode, closedList)){					 
                    openList->addObject(aNode);
				}
			}
            
            
		}
	}
	
	//No Path Found
	return NULL;
}

/* Helper method: Find the lowest cost node in an array */
AStarPathNode* AStarPathNode::lowestCostNodeInArray(CCArray *a) {
	AStarPathNode *lowest = NULL;
	for(int i=0; i<a->count(); i++){
		AStarPathNode *node = (AStarPathNode*) a->objectAtIndex(i);
		if(!lowest || node->cost < lowest->cost){
			lowest = node;
		}
	}
	return lowest;
}

/* Helper method: Is a path node in a given list? */
bool AStarPathNode::isPathNodeinList(AStarPathNode *a, CCArray *list){
	for(int i=0; i<list->count(); i++){
		AStarPathNode *b = (AStarPathNode*) list->objectAtIndex(i);//[list objectAtIndex:i];
		if(a->node->position.x == b->node->position.x && a->node->position.y == b->node->position.y){
			return true;
		}
	}
	return false;
}
