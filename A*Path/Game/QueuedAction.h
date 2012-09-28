//
//  QueuedAction.h
//  Box2dWater
//
//  Created by Dave on 12-8-29.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__QueuedAction__
#define __Box2dWater__QueuedAction__

#include "Base.h"
#import "GameObject.h"
#import <stdlib.h>


class QueuedAction : public CCObject {
    
    
public:
    QueuedAction();
    GameObject* gameObject;
	CCAction* action;
    static QueuedAction* create();
    virtual void release();
    
}; 

 
#endif /* defined(__Box2dWater__QueuedAction__) */
