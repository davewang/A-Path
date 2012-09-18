//
//  GameHelper.cpp
//  Box2dWater
//
//  Created by Dave on 12-8-28.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#include "GameHelper.h"


typedef float coord_t;         // coordinate type
typedef long long coord2_t;  // must be big enough to hold 2*max(|coordinate|)^2

struct Point {
	coord_t x, y;
    
	bool operator <(const Point &p) const {
		return x < p.x || (x == p.x && y < p.y);
	}
    
    
    bool operator ==(const Point &p) const 
    {
        
        printf("A{x:%f,y:%f} B{x:%f,y:%f} =%d\n",x,y,p.x,p.y,(x == p.x && y==p.y));
         
        return x == p.x && y==p.y;
    }
    
};

// 2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
// Returns a positive value, if OAB makes a counter-clockwise turn,
// negative for clockwise turn, and zero if the points are collinear.
coord2_t cross(const Point &O, const Point &A, const Point &B)
{
	return (A.x - O.x) * (coord2_t)(B.y - O.y) - (A.y - O.y) * (coord2_t)(B.x - O.x);
}

// Returns a list of points on the convex hull in counter-clockwise order.
// Note: the last point in the returned list is the same as the first one.
vector<Point> convex_hull(vector<Point> P)
{
	int n = P.size(), k = 0;
	vector<Point> H(2*n);
    
 
	// Sort points lexicographically
	sort(P.begin(), P.end());
 
	// Build lower hull
	for (int i = 0; i < n; i++) {
		while (k >= 2 && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
		H[k++] = P[i];
	}
    
	// Build upper hull
	for (int i = n-2, t = k+1; i >= 0; i--) {
		while (k >= t && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
		H[k++] = P[i];
	}
 
	H.resize(k);
     
	return H;
}


CCArray* GameHelper::convexHull(CCArray *p)
{
    int n = p->count();
	 
	CCArray *H = CCArray::createWithCapacity(n*2);
     
    vector<Point> v;
    for (int i=0; i<n; i++) {
       Vector3D *tmp =(Vector3D*)p->objectAtIndex(i);
       Point tmp_p;
       tmp_p.x = tmp->x;
       tmp_p.y = tmp->y;
      // CCLog("push_back tmp_p{x:%f,y:%f}",tmp_p.x,tmp_p.y);
       v.push_back(tmp_p);
    }
    vector<Point> realVector = convex_hull(v);
    
    
    
    for (int i=0; i<realVector.size(); i++) {
        Point tmp_p = realVector[i];
        //CCLog("convex tmp_p{x:%f,y:%f}",tmp_p.x,tmp_p.y);
        H->addObject(Vector3D::xyz(tmp_p.x, tmp_p.y, 0));
    }
    CCArray *arr = CCArray::create();
    for (int i = 0;i<H->count(); i++) {
        
        for (int j=0; j<arr->count(); j++) {
            Vector3D *a = (Vector3D*)H->objectAtIndex(i);
            Vector3D *b = (Vector3D*)arr->objectAtIndex(j);
            if (a->x==b->x &&a->y==b->y&&a->z==b->z) {
                goto contain;
            }
        }
        if (!arr->containsObject(H->objectAtIndex(i))) {
             arr->addObject( H->objectAtIndex(i));
            // CCLog("containsObject add");
        }
     contain:
        if (1==0)
            CCLog("");
    }
    return arr;
}

bool GameHelper::clockwiseOAB(Vector3D* O,Vector3D* A,Vector3D* B)
{
    return (A->x - O->x) * (B->y - O->y) - (A->y - O->y) * (B->x - O->x) <= 0;

}
float GameHelper::distanceP1toP2(CCPoint p1,CCPoint p2){
    return sqrt( pow( (p1.x-p2.x) ,2) + pow( (p1.y-p2.y) ,2) );
}
float GameHelper::degreesToRadians(float d){
    return d * (PI_CONSTANT/180);
}
float GameHelper::radiansToDegrees(float r){
    return r * (180/PI_CONSTANT);
}
float GameHelper::vectorToRadians(CCPoint vector){
    if(vector.y == 0){ vector.y = 0.000001f; }
	float baseRadians = atan(vector.x/vector.y);
	if(vector.y < 0){ baseRadians += PI_CONSTANT; }	//Adjust for -Y
	return baseRadians;
}
CCPoint GameHelper::radiansToVector(float radians){
    
     	return CCPointMake(sin(radians), cos(radians));
}
Vector3D* GameHelper::quadraticABC(float a,float b,float c){
    
    Vector3D *solution = NULL;
    if (GameHelper::absoluteValue(a) < 1e-6) {
        if (GameHelper::absoluteValue(b) < 1e-6) {
           
            solution = GameHelper::absoluteValue(c) < 1e-6 ?  Vector3D::xyz(0, 0, 0) : NULL;
        } else {
            solution = Vector3D::xyz(-c/b, -c/b , 0);
        }
    } else {
        float disc = b*b - 4*a*c;
        if (disc >= 0) {
            disc = sqrt(disc);
            a = 2*a;
            solution =  Vector3D::xyz((-b-disc)/a,(-b+disc)/a,0);
        }
    }
    return solution;
}
float GameHelper::absoluteValue(float a){
    if(a < 0){ a*= -1; }
	return a;
}
CCPoint GameHelper::midPointP1andP2(CCPoint p1,CCPoint p2){
    return CCPointMake( (p1.x+p2.x)/2 , (p1.y+p2.y)/2 );
}
bool GameHelper::pointIsInRect(CCPoint p,CCRect r){
    bool isInRect = false;
	if( p.x < r.origin.x + r.size.width &&
	   p.x > r.origin.x &&
	   p.y < r.origin.y + r.size.height &&
	   p.y > r.origin.y )
	{
		isInRect = true;
	}
	return isInRect;
}
bool GameHelper::pointIsInCircleWithRadius(CCPoint p,CCPoint origin,float radius){
    bool isInCircle = false;
    
	if(GameHelper::distanceP1toP2(p, origin) <= radius){
		isInCircle = true;
	}
	return isInCircle;
}
b2Vec2 b2ClampALowHigh(const b2Vec2& a,const b2Vec2& low,const b2Vec2& high) {
	return b2Max(low,b2Min(a, high));
}

 
const char* GameHelper::sanitizeString(const char* str){
    string *string1 = new string(str);
    
    string repStr = "'";
    for (int i = 0 ; i<string1->length(); i++) {
        unsigned short c = (unsigned short)string1->at(i);
        if(c >= 2048){
            string1->replace(i , 1, repStr);
        }
    }
	return string1->c_str();
}