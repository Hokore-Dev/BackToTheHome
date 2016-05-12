#include "SplashScene.h"
#include "UserData.h"
#include "MenuScene.h"
USING_NS_CC;

#include "SimpleAudioEngine.h"

using CocosDenshion::SimpleAudioEngine;

SplashScene::SplashScene()
{
}


SplashScene::~SplashScene()
{
}

Scene* SplashScene::createScene()
{
	auto scene = Scene::create();
	auto layer = SplashScene::create();
	scene->addChild(layer);
	return scene;
}

bool SplashScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/main.mp3", true);

	auto logo = Sprite::create("Texture/logo.png");
	logo->setOpacity(0);
	logo->setAnchorPoint(Vec2(0, 0));
	this->addChild(logo);

	logo->runAction(Sequence::create(
		FadeIn::create(1.0f),
		CallFunc::create(std::bind([&]{
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MenuScene::createScene()));
	})),nullptr));
	return true;
}