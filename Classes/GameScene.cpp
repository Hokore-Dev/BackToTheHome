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

GameScene::GameScene() : ballCreateDelta(0), levelBallCount(0),
level(1), levelDelta(0), score(0), targetCharacter(nullptr)
{

}

GameScene::~GameScene()
{
	characters->clear();
	delete characters;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	characters = new std::vector<Character*>;

	size = Director::getInstance()->getWinSize();

	auto background = Sprite::create("Texture/background.png");
	background->setPosition(size.width / 2, size.height / 2);
	this->addChild(background);

	crashRect = Sprite::create("Texture/crashBox.png");
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
	lbScore->setColor(Color3B::BLACK);
	lbScore->setPosition(size.width / 2, size.height - size.height / 5);
	this->addChild(lbScore,5);

	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ALL_AT_ONCE);

	scheduleUpdate();
    return true;
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
			// TODO : GAME OVER
			break;
		}
		// CHECK HOME
		if (crashRect->getBoundingBox().intersectsRect((*itr)->getBoundingBox()))
		{
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

	levelBallCount = level * 3;
	if (ballCreateDelta > 1.0f)
	{
		ballCreateDelta = 0;
		if (characters->size() < levelBallCount)
		{
			Character* character = Character::create((DIRECTION)cocos2d::random(0, 3));
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
		if (v->getBoundingBox().containsPoint(touch))
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
		if (v->getBoundingBox().containsPoint(touch))
		{
			v->rotateCharacter();
		}
	}
}