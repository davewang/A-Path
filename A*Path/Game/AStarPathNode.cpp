//
//  AStarPathNode.cpp
//  A*Path
//
//  Created by Dave on 12-9-11.
//
//

#include "AStarPathNode.h"
#include "Prefix.pch"

AStarPathNode::AStarPathNode():CCObject() {

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
void AStarPathNode::release()
{
    if (node!= NULL)
        node->release();
    if (previous!= NULL)
        previous->release();

    CCObject::release();

}

/* Our implementation of the A* search algorithm */
CCArray*  AStarPathNode::findPathFromTo(AStarNode *fromNode, AStarNode *toNode) {
	CCArray *foundPath = new CCArray(3);
    foundPath->init();
     
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

            while(aNode!=NULL&& NULL != aNode->previous){
				//Mark path
                foundPath->addObject(CCValue::valueWithCCPoint(ccp(aNode->node->position.x, aNode->node->position.y)));
				aNode = aNode->previous;
			}


            foundPath->addObject(CCValue::valueWithCCPoint(ccp(aNode->node->position.x, aNode->node->position.y)));
            openList->removeAllObjects();
            closedList->removeAllObjects();
			return foundPath;
		}else{
			//Still searching
            CCLog(" add before closedList->count() = %d", closedList->count());
            closedList->addObject(currentNode);
            CCLog(" add after closedList->count() = %d", closedList->count());
            CCLog(" remove before openList->count() = %d", openList->count());
            openList->removeObject(currentNode);
            CCLog(" remove after openList->count() = %d", openList->count());
			
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
    openList->removeAllObjects();
    closedList->removeAllObjects();
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
