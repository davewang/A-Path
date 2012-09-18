//
//  QueuedAction.cpp
//  Box2dWater
//
//  Created by Dave on 12-8-29.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#include "QueuedAction.h"
QueuedAction * QueuedAction::create()
{
   
    return new QueuedAction();
}
QueuedAction::QueuedAction()
{
    CCLOG("QueuedAction init .");
}