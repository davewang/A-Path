//
//  Vector3D.h
//  Box2dWater
//
//  Created by Dave on 12-8-28.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__Vector3D__
#define __Box2dWater__Vector3D__

#include "Base.h"
class Vector3D: public CCObject
{
public:
    float x;
    float y;
    float z;

    void setX(float x);
    float getX();
    void setY(float y);
    float getY();
    void setZ(float z);
    float getZ();
    Vector3D();
    Vector3D(float x,float y,float z);
    virtual ~Vector3D();
    virtual bool isEqual(const Vector3D* pObject);
    static bool isVectorEqualTo(Vector3D v1,Vector3D v2);
    static Vector3D* xyz(float x,float y,float z);
    bool operator ==(const Vector3D &p) const;
   
};


#endif /* defined(__Box2dWater__Vector3D__) */
