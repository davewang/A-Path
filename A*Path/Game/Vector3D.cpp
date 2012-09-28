//
//  Vector3D.cpp
//  Box2dWater
//
//  Created by Dave on 12-8-28.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#include "Vector3D.h"
Vector3D::~Vector3D()
{
}
Vector3D::Vector3D()
{
}
Vector3D* Vector3D::xyz(float x, float y, float z)
{

    Vector3D *v = new Vector3D();
    v->x = x;
    v->y = y;
    v->z = z;
    v->autorelease();
    return v;
}
bool Vector3D::operator ==(const Vector3D &p) const
{
    CCLog("a(x:%f,y:%f,z:%f},b(x:%f,y:%f,z:%f}",x,y,z,p.x,p.y,p.z);
    return x == p.x && y==p.y && z == p.z;
}
bool Vector3D::isEqual(const Vector3D* pObject)
{
    return pObject->x==x&&pObject->y==y&&pObject->z==z;
}
bool Vector3D::isVectorEqualTo(Vector3D v1, Vector3D v2)
{
    if(v1.x == v2.x and v1.y == v2.y and v1.z == v2.z){
		return true;
	}
	return false;
}
Vector3D::Vector3D(float x,float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
void Vector3D::setX(float x)
{
    this->x = x;
}
float Vector3D::getX()
{
    return this->x;
}
void Vector3D::setY(float y)
{
    this->y = y;
}
float Vector3D::getY()
{
    return this->y;
}
void Vector3D::setZ(float z)
{
    this->z = z;
}
float Vector3D::getZ()
{
    return this->z;
}