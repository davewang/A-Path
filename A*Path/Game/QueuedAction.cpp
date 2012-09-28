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
    QueuedAction *action = new QueuedAction();
    action->autorelease();
    return action;
}
QueuedAction::QueuedAction()
{
    CCLOG("QueuedAction init .");
}
void QueuedAction::release()
{
    gameObject->release();
    action->release();
    CCObject::release();
}