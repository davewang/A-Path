//
//  GameHelper.h
//  Box2dWater
//
//  Created by Dave on 12-8-28.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__GameHelper__
#define __Box2dWater__GameHelper__
 
#include "Vector3D.h"
#include "Base.h"
#include "Box2D.h"
#include <vector>
#define PI_CONSTANT 3.14159f
typedef enum {
	NO_DIRECTION = 0,
	LEFT = 1,
	UP_LEFT = 2,
	UP = 3,
	UP_RIGHT = 4,
	RIGHT = 5,
	DOWN_RIGHT = 6,
	DOWN = 7,
	DOWN_LEFT = 8
} DirectionType;
class GameHelper : public CCObject
{
 
public:
    // +(NSMutableArray*) convexHull:(NSMutableArray*)P;
    static CCArray* convexHull(CCArray *p);
    static bool clockwiseOAB(Vector3D* O,Vector3D* A,Vector3D* B);
    static float distanceP1toP2(cocos2d::CCPoint p1,cocos2d::CCPoint p2);
    static float degreesToRadians(float d);
    static float radiansToDegrees(float r);
    static float vectorToRadians(cocos2d::CCPoint vector);
    static cocos2d::CCPoint radiansToVector(float radians);
    static Vector3D* quadraticABC(float a,float b,float c);
    static float absoluteValue(float a);
    static cocos2d::CCPoint midPointP1andP2(cocos2d::CCPoint p1,cocos2d::CCPoint p2);
    static bool pointIsInRect(cocos2d::CCPoint p,cocos2d::CCRect r);
    static bool pointIsInCircleWithRadius(cocos2d::CCPoint p,cocos2d::CCPoint origin,float radius);
    static const char* sanitizeString(const char* str);
  

};

#endif /* defined(__Box2dWater__GameHelper__) */
