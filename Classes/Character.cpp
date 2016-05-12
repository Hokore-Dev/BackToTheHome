#include "Character.h"
#include "Config.h"
USING_NS_CC;

Character::Character() : charge(true)
{
}


Character::~Character()
{
}

bool Character::init(DIRECTION direction_)
{
	if (!initWithFile(StringUtils::format("Texture/box/box_%d.png",cocos2d::random(0,2)).c_str()))
	{
		return false;
	}
	this->direction = direction_;
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
	if (direction == LEFT || direction == RIGHT) reachPosition = Vec2(-100 + direction * 200,0);
	else if (direction == DOWN || direction == UP) reachPosition = Vec2(0,-100 + (direction-2) * 200);

	this->runAction(Sequence::create(
		EaseSineOut::create(MoveBy::create(0.5f, reachPosition)),
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