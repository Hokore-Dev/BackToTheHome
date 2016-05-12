#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using CocosDenshion::SimpleAudioEngine;
class SplashScene : public cocos2d::Layer
{
public:
	SplashScene();
	~SplashScene();

	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(SplashScene);
};

#endif