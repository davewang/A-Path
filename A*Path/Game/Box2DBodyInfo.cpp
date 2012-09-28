//
//  Box2DBodyInfo.cpp
//  Box2dWater
//
//  Created by Dave on 12-8-30.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#include "Box2DBodyInfo.h"
//Box2DBodyInfo::Box2DBodyInfo()
//{
//    		//Initialization
//}
Box2DBodyInfo* Box2DBodyInfo::create()
{
  Box2DBodyInfo *info =  new Box2DBodyInfo();
  info->autorelease();
  return info ;
}
void Box2DBodyInfo::release()
{
    delete fixtureDef;
    delete bodyDef;
    gameObject->release();

}