#include "GameScene.h"
#include "Character.h"
#include "Config.h"
USING_NS_CC;

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
	auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

GameScene::GameScene()
{
	/// TODO : CHANGE
	userMoney = 0;
}

GameScene::~GameScene()
{
	characters->clear();
	moneyItems->clear();
	delete moneyItems;
	delete characters;
}

void GameScene::dataInitailize()
{
	ballCreateDelta = 0;
	levelBallCount = 0;
	level = 1;
	levelDelta = 0;
	score = 0;
	targetCharacter = nullptr;
	gameover = false;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	dataInitailize();
	characters = new std::vector<Character*>;
	moneyItems = new std::vector<Sprite*>;

	size = Director::getInstance()->getWinSize();

	auto background = Sprite::create("Texture/background.png");
	background->setRotation(180);
	background->setPosition(size.width / 2, size.height / 2);
	this->addChild(background);

	auto backEffect = ParticleSystemQuad::create("Texture/BackEffect.plist");
	backEffect->setPosition(size.width / 2, size.height / 2 - 100);
	this->addChild(backEffect, 5);

	crashRect = Sprite::create("Texture/crashBox.png");
	crashRect->setVisible(false);
	crashRect->setPosition(size.width / 2, size.height / 2);
	this->addChild(crashRect);

	arrow = Sprite::create("Texture/arrow.png");
	arrow->setOpacity(0);
	this->addChild(arrow);

	home = Sprite::create("Texture/home.png");
	home->setScale(1.5f);
	home->setPosition(size.width / 2, size.height / 2);
	this->addChild(home, 5);

	lbScore = Label::create("0","fonts/calibri.ttf" , 140);
	lbScore->setColor(Color3B::WHITE);
	lbScore->setPosition(size.width / 2, size.height - size.height / 6);
	this->addChild(lbScore,5);

	moneyIcon = Sprite::create("Texture/money.png");
	moneyIcon->setPosition(size.width - 60, size.height - 70);
	this->addChild(moneyIcon,5);

	lbUserMoney = Label::create("0", "fonts/calibri.ttf", 70);
	lbUserMoney->setColor(Color3B::WHITE);
	lbUserMoney->setAnchorPoint(Vec2(1, 0.5));
	lbUserMoney->setPosition(size.width - 95, size.height - 82);
	this->addChild(lbUserMoney, 5);

	bestScore = Label::create("Best Score", "fonts/calibri.ttf", 80);
	bestScore->setColor(Color3B::WHITE);
	bestScore->setPosition(size.width / 2, size.height - size.height / 6 - 150);
	bestScore->setOpacity(0);
	this->addChild(bestScore, 5);

	clearMessage = Sprite::create("Texture/clearMessage.png");
	clearMessage->setPosition(size.width / 2, size.height / 6 + 30 );
	clearMessage->setOpacity(0);
	this->addChild(clearMessage,5);

	ranking = Sprite::create("Texture/ranking.png");
	ranking->setPosition(size.width / 2 + 160, (size.height / 6) - 100);
	ranking->setOpacity(0);
	this->addChild(ranking, 5);

	share = Sprite::create("Texture/share.png");
	share->setPosition(size.width / 2, (size.height / 6) - 100);
	share->setOpacity(0);
	this->addChild(share, 5);

	moneyUp = Sprite::create("Texture/moneyup.png");
	moneyUp->setPosition(size.width / 2 - 160, (size.height / 6) - 100);
	moneyUp->setOpacity(0);
	this->addChild(moneyUp, 5);

	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ALL_AT_ONCE);

	scheduleUpdate();
    return true;
}

void GameScene::gameResultShow()
{
	gameover = true;
	home->runAction(EaseExponentialOut::create(RotateTo::create(0.5f, 0)));
	clearMessage->runAction(
		Sequence::create(DelayTime::create(0.2f),
		Spawn::create(FadeIn::create(0.5f),
		EaseSineOut::create(MoveBy::create(0.5f, Vec2(0, 50))), nullptr),nullptr));
	ranking->runAction(
		Sequence::create(DelayTime::create(0.2f),
		Spawn::create(FadeIn::create(0.5f),
		EaseSineOut::create(MoveBy::create(0.5f, Vec2(0, 50))), nullptr), nullptr));

	share->runAction(
		Sequence::create(DelayTime::create(0.2f),
		Spawn::create(FadeIn::create(0.5f),
		EaseSineOut::create(MoveBy::create(0.5f, Vec2(0, 50))), nullptr), nullptr));

	moneyUp->runAction(
		Sequence::create(DelayTime::create(0.2f),
		Spawn::create(FadeIn::create(0.5f),
		EaseSineOut::create(MoveBy::create(0.5f, Vec2(0, 50))), nullptr), nullptr));

	bestScore->runAction(Spawn::create(FadeIn::create(0.5f),
		EaseSineOut::create(MoveBy::create(0.5f, Vec2(0, 50))), nullptr));
}

void GameScene::update(float delta)
{
	//
	auto itr = characters->begin();
	for (itr; itr != characters->end(); itr++)
	{
		if ((*itr)->isAnimation()) continue;

		// CHECK BACK
		if ((*itr)->getPosition().x < 0 ||
			(*itr)->getPosition().x > 720 ||
			(*itr)->getPosition().y < 0 ||
			(*itr)->getPosition().y > 1280)
		{
			if (targetCharacter == (*itr))
				targetCharacter = nullptr;
			(*itr)->removeFromParent();
			characters->erase(itr);
			levelBallCount--;
			unscheduleUpdate();
			gameResultShow();
			break;
		}
		// CHECK HOME
		if (crashRect->getBoundingBox().intersectsRect((*itr)->getBoundingBox()))
		{
			auto particle = ParticleSystemQuad::create("Texture/crashEffect.plist");
			particle->setPosition((*itr)->getPosition());
			this->addChild(particle,5);

			if ((*itr)->isMoney())
			{
				userMoney++;
				lbUserMoney->setString(StringUtils::toString(userMoney));
			}
			if (targetCharacter == (*itr))
				targetCharacter = nullptr;
			(*itr)->removeFromParent();
			characters->erase(itr);
			levelBallCount--;
			score++;

			lbScore->stopAllActions();
			lbScore->runAction(Sequence::create(
				EaseBackOut::create(ScaleTo::create(0.2f,1.2f)),
				EaseSineOut::create(ScaleTo::create(0.1f,1.0f)), nullptr));

			lbScore->setString(StringUtils::toString(score));
			break;
		}
	}
	// 
	ballCreateDelta += delta;
	levelDelta		+= delta;

	levelBallCount = level * LEVEL_BALL_RANGE;
	if (ballCreateDelta > BALL_CREATE_TIME)
	{
		ballCreateDelta = 0;
		if (characters->size() < levelBallCount)
		{
			DIRECTION randomDir = (DIRECTION)cocos2d::random(0, 3);
			int rotateValue = 0;
			switch (randomDir)
			{
			case LEFT: rotateValue = 90; break;
			case RIGHT: rotateValue = 270; break;
			case DOWN: rotateValue = 0;break;
			case UP:rotateValue = 180;break;
			default:
				break;
			}
			home->runAction(EaseExponentialOut::create(RotateTo::create(0.5f, rotateValue)));
			Character* character = Character::create(randomDir);
			character->setPosition(size.width / 2, size.height / 2);
			this->addChild(character);
			characters->push_back(character);
		}
	}
	if (levelDelta > LEVEL_DELTA)
	{
		levelDelta = 0;
		level++;
	}
	//
	if (targetCharacter != nullptr)
	{
		arrow->setPosition(targetCharacter->getPositionX(), targetCharacter->getPositionY() + 70);
	}
}

void GameScene::onTouchesBegan(const std::vector<Touch*> &touches, Event* unused_event)
{
	Vec2 touch = touches[0]->getLocation();
	for (Character* v : *characters)
	{
		if (v->getBoundingBox().containsPoint(touch) && v->isCharge())
		{
			targetCharacter = v;
			arrow->stopAllActions();
			arrow->runAction(FadeIn::create(0.1f));
			switch (v->getDirection())
			{
			case LEFT:arrow->setRotation(90); break;
			case RIGHT:arrow->setRotation(270); break;
			case DOWN:arrow->setRotation(0); break;
			case UP:arrow->setRotation(180); break;
			default:
				break;
			}
		}
	}
}
void GameScene::onTouchesMoved(const std::vector<Touch*> &touches, Event* unused_event)
{

}
void GameScene::onTouchesEnded(const std::vector<Touch*> &touches, Event* unused_event)
{
	arrow->stopAllActions();
	arrow->runAction(FadeOut::create(0.1f));

	Vec2 touch = touches[0]->getLocation();
	for (Character* v : *characters)
	{
		if (v->getBoundingBox().containsPoint(touch) && v->isCharge())
		{
			v->rotateCharacter();
		}
	}

	if (gameover)
	{
		for (Character* v : *characters)
		{
			v->removeFromParentAndCleanup(true);
		}
		lbScore->setString("0");
		characters->clear();
		dataInitailize();
		scheduleUpdate();
		clearMessage->runAction(Sequence::create(DelayTime::create(0.2f),
			Spawn::create(FadeOut::create(0.5f),
			EaseSineOut::create(MoveBy::create(0.5f,Vec2(0,-50))),nullptr),nullptr));
		bestScore->runAction(Spawn::create(FadeOut::create(0.5f),
			EaseSineOut::create(MoveBy::create(0.5f, Vec2(0, -50))), nullptr));
		ranking->runAction(
			Sequence::create(DelayTime::create(0.2f),
			Spawn::create(FadeOut::create(0.5f),
			EaseSineOut::create(MoveBy::create(0.5f, Vec2(0, -50))), nullptr), nullptr));
		share->runAction(
			Sequence::create(DelayTime::create(0.2f),
			Spawn::create(FadeOut::create(0.5f),
			EaseSineOut::create(MoveBy::create(0.5f, Vec2(0, -50))), nullptr), nullptr));
		moneyUp->runAction(
			Sequence::create(DelayTime::create(0.2f),
			Spawn::create(FadeOut::create(0.5f),
			EaseSineOut::create(MoveBy::create(0.5f, Vec2(0, -50))), nullptr), nullptr));

	}
}