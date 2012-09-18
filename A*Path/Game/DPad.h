//
//  DPad.h
//  Box2dWater
//
//  Created by Dave on 12-9-2.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__DPad__
#define __Box2dWater__DPad__

#include "Base.h"
#include "TouchableSprite.h"
class DPad : public TouchableSprite
{
public:
    CCPoint pressedVector;
    int direction;
    
    
    DPad();
    void processTouchFromDirection(int dir);
    void processTouch(CCPoint point);
    void processRelease();
};

#endif /* defined(__Box2dWater__DPad__) */
