#include "Character.h"
#include "Config.h"
#include "UserData.h"
USING_NS_CC;

Character::Character() : charge(true), money(false), choose(false)
{
}


Character::~Character()
{
}

bool Character::init(DIRECTION direction_)
{
	if (!cocos2d::random(0, 5))
	{
		money = true;
	}
	if (!initWithFile(money ? "Texture/money.png" :
		StringUtils::format("Texture/%s/%d.png",
		UserData::getInstance()->getSelectCharacter().c_str(),
		cocos2d::random(0,2)).c_str()))
	{
		return false;
	}
	if (money) this->setScale(1.6f);
	else this->setScale(1.5f);

	this->direction = direction_;
	this->setOpacity(180);
	initAnimation();

	chargeBarBack = Sprite::create("Texture/loadbox.png");
	chargeBarBack->setPosition(this->getContentSize().width/2,75);
	chargeBarBack->setCascadeOpacityEnabled(true);
	chargeBarBack->setOpacity(0);
	this->addChild(chargeBarBack);

	chargeBar = Sprite::create("Texture/loadboxbar.png");
	chargeBar->setAnchorPoint(Vec2(0, 0.5));
	chargeBar->setPosition(1, chargeBarBack->getContentSize().height / 2);
	chargeBarBack->addChild(chargeBar);

	return true;
}

void Character::initAnimation()
{
	this->animation = true;
	Vec2 reachPosition = Vec2::ZERO;
	if (direction == LEFT || direction == RIGHT) reachPosition = Vec2(-120 + direction * 240,0);
	else if (direction == DOWN || direction == UP) reachPosition = Vec2(0,-120 + (direction-2) * 240);

	this->runAction(Sequence::create(
		EaseSineOut::create(MoveBy::create(0.5, reachPosition)),
		CallFunc::create(std::bind([&]{
		scheduleUpdate();
		this->animation = false;
	})), nullptr));
}

bool Character::rotateCharacter()
{
	if (!charge) return false;
	switch (direction)
	{
	case LEFT:	direction = DOWN;  break;
	case RIGHT: direction = UP;  break;
	case DOWN:	direction = RIGHT; break;
	case UP:	direction = LEFT;  break;
	default:
		break;
	}
	charge = false;
	chargeBar->setScaleX(0);
	chargeBarBack->runAction(FadeIn::create(0.2f));

	return true;
}

void Character::update(float delta)
{
	if (chargeBar->getScaleX() < 1)
	{
		chargeBar->setScaleX(chargeBar->getScaleX() + BALL_CHARGEBAR_INCREASE_VALUE);
		if (chargeBar->getScaleX() >= 1)
		{
			chargeBar->setScaleX(1);
			charge = true;
			chargeBarBack->runAction(FadeOut::create(0.2f));
		}
	}
	switch (direction)
	{
	case LEFT:setPositionX(getPositionX() - BALL_SPEED); break;
	case RIGHT:setPositionX(getPositionX() + BALL_SPEED); break;
	case DOWN:setPositionY(getPositionY() - BALL_SPEED); break;
	case UP:setPositionY(getPositionY() + BALL_SPEED); break;
	default:break;
	}
}