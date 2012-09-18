//
//  TouchableSprite.cpp
//  Box2dWater
//
//  Created by Dave on 12-9-2.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#include "TouchableSprite.h"
#include "GameHelper.h"
TouchableSprite::TouchableSprite()
{
    CCSprite::CCSprite();
    CCSprite::init();
    pressed = false;
}


bool TouchableSprite::checkTouchWithPoint(CCPoint point)
{
    
	if( GameHelper::pointIsInRect(point, this->rect())){
		return true;
	}else{
		return false;
	}
}

CCRect TouchableSprite::rect()
{
	//We set our scale mod to make sprite easier to press.
	//This also lets us press 2 sprites with 1 touch if they are sufficiently close.
	float scaleMod = 1.5f;
	float w = this->getContentSize().width * this->getScale() * scaleMod;
	float h = this->getContentSize().height * this->getScale()  * scaleMod;
	CCPoint point = CCPointMake(this->getPosition().x - (w/2),this->getPosition().y - (h/2));
	
	return CCRectMake(point.x, point.y, w, h);
}




void TouchableSprite::ccTouchesBegan(CCSet *touches ,CCEvent *event)
{
	CCTouch *touch = (CCTouch*)touches->anyObject();
    CCPoint point =  touch->getLocationInView();
	point = CCDirector::sharedDirector()->convertToGL(point);
    
    
	//We use circle collision for our buttons
	if( GameHelper::pointIsInCircleWithRadius(point,this->getPosition(), this->rect().size.width/2))
    {
		touchHash = touch->m_uID;
		this->processTouch(point);
	}
}

void TouchableSprite::ccTouchesMoved(CCSet *touches ,CCEvent *event)
{
	CCTouch *touch = (CCTouch*)touches->anyObject();
    CCPoint point =  touch->getLocationInView();
	point = CCDirector::sharedDirector()->convertToGL(point);
    
	//if([GameHelper point:point isInCircle:self.position withRadius:self.rect.size.width/2])
    if( GameHelper::pointIsInCircleWithRadius(point,this->getPosition(), this->rect().size.width/2))
    {
		if(touchHash == touch->m_uID){		//If we moved on this sprite
			this->processTouch(point);
             
		}else if(!pressed){					//If a new touch moves onto this sprite
			touchHash = touch->m_uID;
            this->processTouch(point);
		}
	}else if(touchHash == touch->m_uID){	//If we moved off of this sprite
        this->processRelease();
	}
}

void TouchableSprite::ccTouchesEnded(CCSet *touches ,CCEvent *event) {
	CCTouch *touch = (CCTouch*)touches->anyObject();
    CCPoint point =  touch->getLocationInView();
	point = CCDirector::sharedDirector()->convertToGL(point);
    
	 
    if(touchHash == touch->m_uID){	//If the touch which pressed this sprite ended we release
        this->processRelease();
	}
}

void TouchableSprite::processTouch(CCPoint point) {
	pressed = true;
}

void TouchableSprite::processRelease() {
	pressed = false;
}