//
//  GameMisc.h
//  Box2dWater
//
//  Created by Dave on 12-8-30.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__GameMisc__
#define __Box2dWater__GameMisc__
#include "GameObject.h"
class GameMisc : public GameObject{
public:
    float life;
    GameMisc();
    virtual int type();
};

#endif /* defined(__Box2dWater__GameMisc__) */
