#include "MenuScene.h"
#include "UserData.h"
#include "GameScene.h"
#include "Config.h"
#include "InventoryLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform\android\jni\JniHelper.h"
#endif
USING_NS_CC;

MenuScene::MenuScene()
{
}


MenuScene::~MenuScene()
{
}

Scene* MenuScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MenuScene::create();
	scene->addChild(layer);
	return scene;
}

bool MenuScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t,
		"org/cocos2dx/cpp/AppActivity",
		"unityAdsInit",
		"()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
	}
#endif

	UserData::getInstance()->setBackgroundCount(cocos2d::random(0, MAP_COUNT - 1));

	size = Director::getInstance()->getWinSize();

	auto background = Sprite::create(StringUtils::format(
		"Texture/background/background_%d.png", 
		UserData::getInstance()->getBackgroundCount()));
	background->setRotation(180);
	background->setPosition(size.width / 2, size.height / 2);
	this->addChild(background);

	auto backEffect = ParticleSystemQuad::create("Texture/menu_back.plist");
	backEffect->setPosition(size.width / 2, size.height / 2 + 100);
	this->addChild(backEffect);

	title = Sprite::create("Texture/title.png");
	title->setPosition(size.width / 2, size.height / 2 );
	title->setOpacity(0);
	this->addChild(title);

	Menu* menu = Menu::create();
	menu->setPosition(Vec2::ZERO);

	start = MenuItemImage::create("Texture/start.png",
		"Texture/start.png", CC_CALLBACK_1(MenuScene::transGameScene, this));
	start->setPosition(size.width / 2, size.height / 2 - 100);
	start->setOpacity(0);
	menu->addChild(start);

	sound = MenuItemImage::create("Texture/sound.png",
		"Texture/sound.png", std::bind([&]{
		SimpleAudioEngine::getInstance()->playEffect("Sound/touch.wav");
		UserData::getInstance()->setSound(!UserData::getInstance()->getSound());
		if (UserData::getInstance()->getSound())
		{
			sound->setSelectedImage(Sprite::create("Texture/sound.png"));
			sound->setNormalImage(Sprite::create("Texture/sound.png"));
			SimpleAudioEngine::getInstance()->setEffectsVolume(0.5f);
			SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5f);
		}
		else
		{
			SimpleAudioEngine::getInstance()->setEffectsVolume(0);
			SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
			sound->setSelectedImage(Sprite::create("Texture/unsound.png"));
			sound->setNormalImage(Sprite::create("Texture/unsound.png"));
		}
	}));
	sound->setPosition(size.width / 2 - 200, size.height / 2 - 500 - 500);
	menu->addChild(sound);

	if (UserData::getInstance()->getSound())
	{
		sound->setSelectedImage(Sprite::create("Texture/sound.png"));
		sound->setNormalImage(Sprite::create("Texture/sound.png"));
		SimpleAudioEngine::getInstance()->setEffectsVolume(0.5f);
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5f);
	}
	else
	{
		SimpleAudioEngine::getInstance()->setEffectsVolume(0);
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
		sound->setSelectedImage(Sprite::create("Texture/unsound.png"));
		sound->setNormalImage(Sprite::create("Texture/unsound.png"));
	}

	store = MenuItemImage::create("Texture/store.png",
		"Texture/store.png", std::bind([&]{
		SimpleAudioEngine::getInstance()->playEffect("Sound/touch.wav");
		this->addChild(InventoryLayer::create());
	}));
	store->setPosition(size.width / 2, size.height / 2 - 500 - 500);
	menu->addChild(store);

	ranking = MenuItemImage::create("Texture/ranking.png",
		"Texture/ranking.png", std::bind([&]{
		SimpleAudioEngine::getInstance()->playEffect("Sound/touch.wav");
	}));
	ranking->setPosition(size.width / 2 + 200, size.height / 2 - 500 - 500);
	menu->addChild(ranking);

	this->addChild(menu);

	moneyIcon = Sprite::create("Texture/money.png");
	moneyIcon->setOpacity(0);
	moneyIcon->setPosition(size.width - 60, size.height - 70);
	this->addChild(moneyIcon, 5);

	lbUserMoney = Label::create(StringUtils::toString(UserData::getInstance()->getMoney()),
		"fonts/calibri.ttf", 70);
	lbUserMoney->setColor(Color3B::WHITE);
	lbUserMoney->setOpacity(0);
	lbUserMoney->setAnchorPoint(Vec2(1, 0.5));
	lbUserMoney->setPosition(size.width - 95, size.height - 82);
	this->addChild(lbUserMoney, 5);
	this->setKeypadEnabled(true);                 

	exitMessage = Sprite::create("Texture/exitMessage.png");
	exitMessage->setOpacity(0);
	exitMessage->setPosition(size.width / 2, 300);
	this->addChild(exitMessage,5);

	initAnimation();
	return true;
}

void MenuScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode,
	cocos2d::Event * event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK){
		if (exitMessage->getOpacity() == 0)
		{
			exitMessage->runAction(Sequence::create(
				FadeIn::create(1.0f),
				FadeOut::create(1.0f), nullptr));
		}
		else
		{
			Director::getInstance()->end();
		}
	}
}

void MenuScene::initAnimation()
{
	title->runAction(FadeIn::create(0.5f));
	start->runAction(FadeIn::create(0.5f));
	moneyIcon->runAction(FadeIn::create(0.5f));
	lbUserMoney->runAction(FadeIn::create(0.5f));

	sound->runAction(EaseQuarticActionOut::create(MoveBy::create(0.5f, Vec2(0, 500))));
	store->runAction(EaseQuarticActionOut::create(MoveBy::create(0.5f, Vec2(0, 500))));
	ranking->runAction(EaseQuarticActionOut::create(MoveBy::create(0.5f, Vec2(0, 500))));
}

void MenuScene::destroyAnimation()
{
	title->runAction(FadeOut::create(0.7f));
	start->runAction(FadeOut::create(0.7f));
	moneyIcon->runAction(FadeOut::create(0.7f));
	lbUserMoney->runAction(FadeOut::create(0.7f));

	sound->runAction(EaseQuarticActionOut::create(MoveBy::create(2.0f, Vec2(0, -500))));
	store->runAction(EaseQuarticActionOut::create(MoveBy::create(2.0f, Vec2(0, -500))));
	ranking->runAction(EaseQuarticActionOut::create(MoveBy::create(2.0f, Vec2(0, -500))));
}

void MenuScene::transGameScene(Ref* sender)
{
	destroyAnimation();

	this->runAction(Sequence::create(
		DelayTime::create(0.7f),
		CallFunc::create(std::bind([&]{
		Director::getInstance()->replaceScene(GameScene::createScene());
	})), nullptr));
	SimpleAudioEngine::getInstance()->playEffect("Sound/touch.wav");
}