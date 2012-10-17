//
//  Recipe.cpp
//  Box2dWater
//
//  Created by Dave on 12-8-24.
//  Copyright (c) 2012年 DaveDev. All rights reserved.
//

#include "Recipe.h" 

Recipe::Recipe():CCLayer()
{
    CCLog("Recipe init");
}
CCLayer* Recipe::runRecipe()
{
    setTouchEnabled(true);
    
	CCSize screenSize =CCDirector::sharedDirector()->getWinSize();
      
    this->message = CCLabelTTF::create("","fonts/huakangfont.ttf", 24); // CCLabelBMFont::create("", "avqest.fnt");    //eurostile_30.fnt
    message->setPosition(ccp(10, screenSize.height-35));
    message->setAnchorPoint(ccp(0,1));
    message->setScale(0.5f);
    message->setColor(ccc3(255, 255, 255));
    this->addChild(message, 10);
    
    this->runAction(CCSequence::create(CCDelayTime::create(5.0f),CCCallFunc::create(this, callfunc_selector(Recipe::resetMessage)),NULL));
	//Remove message after 5 seconds
	//[self runAction:[CCSequence actions:[CCDelayTime actionWithDuration:5.0f],
      //               [CCCallFunc actionWithTarget:self selector:@selector(resetMessage)], nil]];
    
	 

    return this;
}
void Recipe::resetMessage()
{
	
    message->setString("");
}

void Recipe::cleanRecipe()
{
    this->removeAllChildrenWithCleanup(true);
}
 
void Recipe::showMessage(const char *m)
{
    this->stopAllActions();
    m = GameHelper::sanitizeString(m);
    message->setString(m);
    this->runAction(CCSequence::create(CCDelayTime::create(500.0f),CCCallFunc::create(this, callfunc_selector(Recipe::resetMessage)),NULL));
   
}
void Recipe::appendMessage(const char *m)
{
    this->stopAllActions();
    CCLOG(">>>>%s",m);
    m = GameHelper::sanitizeString(m);
    string str = message->getString();
    CCLOG(">>>>%s",str.c_str());
    str+= "\n";
    str+= m;
    message->setString(str.c_str());
}
void Recipe::appendMessageWithWrap(const char *m)
{
    
    this->stopAllActions();
	
	//Check for bad characters
	m = GameHelper::sanitizeString(m);
	
 
    string *str =  new string(m);
    
    string s0 ("\n");
	for(int i=0; i<str->length() ; i+=40){
		while(i < str->length() && str->at(i) != 32){
			i++;
		}
		if(i >= str->length()){
			break;
		}
		//[str insertString:@"\n" atIndex:i];
        str->insert(i,s0);
       
	}
//    str->append("\n");
//    str->append(m );
    string s = "";
    s += message->getString();
    s += "\n";
    s += str->substr();
	message->setString(s.c_str());
	 
}
void Recipe::drawLayer()
{

}