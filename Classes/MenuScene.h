#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using CocosDenshion::SimpleAudioEngine;
class MenuScene : public cocos2d::Layer
{
public:
	MenuScene();
	~MenuScene();

	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MenuScene);

	
private:
	void initAnimation();
	void destroyAnimation();

	void transGameScene(cocos2d::Ref* sender);

	cocos2d::Size size;

	cocos2d::Sprite* title;
	cocos2d::MenuItemImage* start;
	cocos2d::MenuItemImage* sound;
	cocos2d::MenuItemImage* store;
	cocos2d::MenuItemImage* ranking;

	cocos2d::Sprite * moneyIcon;
	cocos2d::Label  * lbUserMoney;
};

#endif