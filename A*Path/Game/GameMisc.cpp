//
//  GameMisc.cpp
//  Box2dWater
//
//  Created by Dave on 12-8-30.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#include "GameMisc.h"
int GameMisc::type()
{
	return GO_TYPE_MISC;
}
GameMisc::GameMisc():GameObject()
{
   // GameObject::GameObject();
}
void GameMisc::release()
{
    GameObject::release();
}
