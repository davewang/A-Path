//
//  DrawingSprites.cpp
//  Box2dWater
//
//  Created by Dave on 12-8-24.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#include "DrawingSprites.h"

using namespace cocos2d;
 
CCScene* DrawingSprites::scene()
{

    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    DrawingSprites *layer = new DrawingSprites(); //(DrawingSprites*)DrawingSprites::node();//DrawingSprites::node();
    layer->init();
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;

}


bool DrawingSprites::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
     
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                                                       "CloseNormal.png",
                                                                       "CloseSelected.png",
                                                                       this,
                                                                       menu_selector(DrawingSprites::addMessage) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );
    
    // create menu, it's an autorelease object
    
    CCMenu* pMenu = CCMenu::createWithItem( pCloseItem);//CCMenu::menuWithItems(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    //this->addChild(pMenu, 1);

    
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Thonburi", 34);
    
    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    // position the label on the center of the screen
    pLabel->setPosition( ccp(size.width / 2, size.height - 20) );
    
  
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");
     pSprite->setPosition( ccp(size.width/2, size.height/2) );
 
   // recipe = new Waypoints();
    //recipe = new AIFlocking();
    recipe = new GridPathfinding();
   
    this->addChild(recipe->runRecipe());
    
    
    return true;
}
void DrawingSprites::addMessage()
{
    recipe->appendMessage("Dave");
}
void DrawingSprites::onEnter()
{
  recipe->appendMessage("Wang");
   CCLayer::onEnter();
}
void DrawingSprites::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}