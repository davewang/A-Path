//
//  TouchableSprite.h
//  Box2dWater
//
//  Created by Dave on 12-9-2.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__TouchableSprite__
#define __Box2dWater__TouchableSprite__

#include "Base.h"

class TouchableSprite :public CCSprite
{
public:
    bool pressed;			//Is this sprite pressed
    int touchHash;	//Used to identify individual touches
  
    TouchableSprite();
    virtual bool checkTouchWithPoint(CCPoint point);
    CCRect rect();
    virtual void processTouch(CCPoint point);
    virtual void processRelease();
    virtual void ccTouchesBegan(CCSet *touches ,CCEvent *event);
    virtual void ccTouchesMoved(CCSet *touches ,CCEvent *event);
    virtual void ccTouchesEnded(CCSet *touches ,CCEvent *event);
};
#endif /* defined(__Box2dWater__TouchableSprite__) */
