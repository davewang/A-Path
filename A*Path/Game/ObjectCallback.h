//
//  ObjectCallback.h
//  Box2dWater
//
//  Created by Dave on 12-8-28.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__ObjectCallback__
#define __Box2dWater__ObjectCallback__

#include "Base.h"
#import <stdlib.h>

class ObjectCallback : public CCObject {
public:
    CCObject *obj;
	const char* callback;
    SEL_CallFunc  callbackfun;
    static ObjectCallback* createWithObjectWithCallback(CCObject *o,const char* callbackString);
    ObjectCallback* initWithObjectWithCallback(CCObject *o,const char* callbackString);
    static ObjectCallback* createWithObjectWithCallback(CCObject *o,SEL_CallFunc callbackfun);
    ObjectCallback* initWithObjectWithCallback(CCObject *o,SEL_CallFunc callbackString);
};
 
#endif /* defined(__Box2dWater__ObjectCallback__) */
