//
//  GameSensor.h
//  Box2dWater
//
//  Created by Dave on 12-8-30.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__GameSensor__
#define __Box2dWater__GameSensor__
#import "GameObject.h"
class GameSensor:public GameObject{
public:
    virtual  int type();
};
#endif /* defined(__Box2dWater__GameSensor__) */
