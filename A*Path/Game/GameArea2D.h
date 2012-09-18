//
//  GameArea2D.h
//  Box2dWater
//
//  Created by Dave on 12-8-28.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#ifndef __Box2dWater__GameArea2D__
#define __Box2dWater__GameArea2D__

#include "Base.h" 
#include "GLES-Render.h"
#include "Recipe.h"
#include "DebugDrawNode.h"
#include "GameObject.h"
#include "GameMisc.h"
#include "ObjectCallback.h"
#include "QueuedAction.h"
#define PTM_RATIO 32
class basicContactFilter;
class basicContactListener;
enum {
	GO_TAG_WALL = 0,
	GO_TAG_ACTOR = 1
};

#define TAP_VS_DRAG_DIST 30.0f

enum {
	GA_CAMERA_STATE_NONE = 0,
	GA_CAMERA_STATE_PANNING = 1,
	GA_CAMERA_STATE_ZOOMING = 2
};

class  GameArea2D : public Recipe
{
    
public:
	b2World* world;
   // GLESDebugDraw *m_debugDraw;
	GLESDebugDraw *m_debugDraw;
	
	CCNode *gameNode;
	DebugDrawNode *debugDrawNode;
	GameObject *levelBoundary;
	 
	CCArray *bodiesToDestroy;
	CCArray *postDestructionCallbacks;
	CCArray *bodiesToCreate;
	CCArray *queuedActions;
	
	CCPoint gameAreaSize;
	float cameraZoom;	//Starts at 1.0f. Smaller means more zoomed out.
	float cameraZoomTo;
	float cameraZoomSpeed; //Must be greater than 0 but less than 1
	float lastMultiTouchZoomDistance;	//How far apart your fingers were last time
	CCPoint draggedToPoint;	//How far we dragged from initial touch
	CCPoint lastTouchedPoint;	//Where we last touched
	int cameraState;	//What is the camera currently doing?
	 
    CCDictionary *allTouches;

    bool debugDraw;
    GameArea2D();
    virtual CCLayer* runRecipe();
    

    virtual void step(float dt);
    virtual void draw();
    virtual void handleCollisionWithObjAWithObjB(GameObject* objA,GameObject*objB);
    
    virtual void showDebugDraw();
    virtual void showMinimalDebugDraw();
    virtual void hideDebugDraw();
    virtual void initDebugDraw();
    virtual void swapDebugDraw();
    virtual void addLevelBoundaries();
    virtual void addRandomPolygons(int num);
    virtual void addRandomBoxes(int num);
    virtual void addPolygonAtPoint(CCPoint p);
    virtual void addBoxAtPointAndSize(CCPoint p,CCPoint s);
    
    virtual void markBodyForDestruction(GameObject* obj);
    virtual void destroyBodies();
    virtual void markBodyForCreation(GameObject* obj);
    virtual void createBodies();
    virtual void runQueuedActions();
    
    //Drawing
    virtual void drawLayer();
    
    //Camera
    virtual bool checkCameraBoundsWithFailPosition(CCPoint failPosition);
    virtual bool checkCameraBounds();
    virtual void setCameraPosition(CCPoint position);
    virtual CCPoint convertCamera(CCPoint p);
    virtual void setCameraZoom(float zoom);
    virtual void zoomToWithSpeed(float zoom,float speed);
    virtual void processZoomStep();
    virtual CCPoint convertTouchCoord(CCPoint touchPoint);
    virtual CCPoint convertGameCoord(CCPoint gamePoint);
    virtual void centerCameraOnGameCoord(CCPoint gamePoint);
  
    //Touches
    virtual void ccTouchesBegan(CCSet *touches ,CCEvent *event);
    virtual void ccTouchesMoved(CCSet *touches ,CCEvent *event);
    virtual void ccTouchesEnded(CCSet *touches ,CCEvent *event);
    virtual bool hudBegan(CCSet *touches ,CCEvent *event);
    virtual bool hudMoved(CCSet *touches ,CCEvent *event);
    virtual bool hudEnded(CCSet *touches ,CCEvent *event);
    virtual void tapWithPoint(CCPoint point);
    
    //Misc
    virtual float distanceP1toP2(CCPoint firstTouch ,CCPoint secondTouch);

};


#endif /* defined(__Box2dWater__GameArea2D__) */
