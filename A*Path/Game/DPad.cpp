//
//  DPad.cpp
//  Box2dWater
//
//  Created by Dave on 12-9-2.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#include "DPad.h"
#include "GameHelper.h"
DPad::DPad()
{
    TouchableSprite::TouchableSprite();
    pressedVector = ccp(0,0);
    direction = NO_DIRECTION;
    
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
   // cache->addSpriteFramesWithFile("dpad_buttons.plist");
      
    
    //Set the sprite display frame
    this->setDisplayFrame( cache->spriteFrameByName("d_pad_normal.png"));
}





void DPad::processTouchFromDirection(int dir)
{
	//NSLog(@"TOUCH DIR");
	
	if(dir == NO_DIRECTION){
        this->processRelease();
    }else if(dir == LEFT){
        this->processTouch(ccp(-50,0));
	}else if(dir == UP_LEFT){
		this->processTouch(ccp(-50,50));
	}else if(dir == UP_LEFT){
		this->processTouch(ccp(-50,50));
	}else if(dir == UP){
		this->processTouch(ccp(0,50));
	}else if(dir == UP_RIGHT){
		this->processTouch(ccp(50,50));
	}else if(dir == RIGHT){
		this->processTouch(ccp(50,0));
	}else if(dir == DOWN_RIGHT){
		this->processTouch(ccp(50,-50));
	}else if(dir == DOWN){
		this->processTouch(ccp(0,-50));
	}else if(dir == DOWN_LEFT){
		this->processTouch(ccp(-50,-50));
	}
}

void DPad::processTouch(CCPoint point)
{
	//NSLog(@"Processing touch");
  
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	
	//Set a color visual cue if pressed
     
	this->setColor(ccc3(255,200,200));
    TouchableSprite::processTouch(point);
    
	CCPoint center = CCPointMake( this->rect().origin.x+this->rect().size.width/2, this->rect().origin.y+this->rect().size.height/2 );
    
	//Process center dead zone
	if(GameHelper::distanceP1toP2(point, center) < this->rect().size.width/10){
		//NSLog(@"NO TOUCH (dead zone)");
        this->setDisplayFrame(cache->spriteFrameByName( "d_pad_normal.png"));
		
		this->setRotation(0);
		pressedVector = ccp(0,0);
		direction = NO_DIRECTION;
        
       
		return;
	}
	
	//Process direction
	float radians = GameHelper::vectorToRadians(CCPointMake(point.x-center.x, point.y-center.y)); 
	float degrees = GameHelper::radiansToDegrees(radians)+90;
    
	float sin45 = 0.7071067812f;
	
	if(degrees >= 337.5 || degrees < 22.5){
        this->setDisplayFrame( cache->spriteFrameByName("d_pad_horizontal.png"));
		
		 
		this->setRotation(180);
        pressedVector = ccp(-1,0);
        direction = LEFT;
        
     
	}else if(degrees >= 22.5 && degrees < 67.5){
        this->setDisplayFrame( cache->spriteFrameByName( "d_pad_diagonal.png"));
		this->setRotation(-90);
         pressedVector = ccp(-sin45,sin45);
        direction = UP_LEFT;
	}else if(degrees >= 67.5 && degrees < 112.5){
		
        this->setDisplayFrame( cache->spriteFrameByName( "d_pad_horizontal.png"));
		this->setRotation(-90);
        
        pressedVector = ccp(0,1); direction = UP;
	}else if(degrees >= 112.5 && degrees < 157.5){
		
        
        this->setDisplayFrame( cache->spriteFrameByName( "d_pad_diagonal.png"));
		this->setRotation(0);
        pressedVector = ccp(sin45,sin45); direction = UP_RIGHT;
	}else if(degrees >= 157.5 && degrees < 202.5){
		 
        this->setDisplayFrame( cache->spriteFrameByName( "d_pad_horizontal.png"));
		this->setRotation(0);
        pressedVector = ccp(1,0); direction = RIGHT;
	}else if(degrees >= 202.5 && degrees < 247.5){
		 
        this->setDisplayFrame( cache->spriteFrameByName( "d_pad_diagonal.png"));
		this->setRotation(90);
        pressedVector = ccp(sin45,-sin45); direction = DOWN_RIGHT;
	}else if(degrees >= 247.5 && degrees < 292.5){
		
        this->setDisplayFrame( cache->spriteFrameByName( "d_pad_horizontal.png"));
		this->setRotation(90);
        pressedVector = ccp(0,-1); direction = DOWN;
	}else{
		 
        this->setDisplayFrame( cache->spriteFrameByName( "d_pad_diagonal.png"));
		this->setRotation(180);
        pressedVector = ccp(-sin45,-sin45); direction = DOWN_LEFT;
	}
}

void DPad::processRelease (){
	 
	this->setColor(ccc3(255,255,255));
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    this->setDisplayFrame( cache->spriteFrameByName( "d_pad_normal.png"));
    this->setRotation(0);
  
    TouchableSprite::processRelease();
	pressedVector = ccp(0,0);
	direction = NO_DIRECTION;
}
