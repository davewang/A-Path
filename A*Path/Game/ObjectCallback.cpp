//
//  ObjectCallback.cpp
//  Box2dWater
//
//  Created by Dave on 12-8-28.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#include "ObjectCallback.h"
ObjectCallback* ObjectCallback::createWithObjectWithCallback(CCObject *o,const char* callbackString)
{
    ObjectCallback *callBack = new ObjectCallback();
    return callBack->initWithObjectWithCallback(o, callbackString) ;
}
ObjectCallback* ObjectCallback::initWithObjectWithCallback(CCObject *o,const char* callbackString)
{
    this->obj = o;
    this->callback = callbackString;
    return this;
}
ObjectCallback* ObjectCallback::createWithObjectWithCallback(CCObject *o,SEL_CallFunc callbackString)
{
    ObjectCallback *callBack = new ObjectCallback();
    return callBack->initWithObjectWithCallback(o, callbackString) ;
}
ObjectCallback* ObjectCallback::initWithObjectWithCallback(CCObject *o,SEL_CallFunc callbackString)
{
    this->obj = o;
    this->callbackfun = callbackString;
    return this;
}