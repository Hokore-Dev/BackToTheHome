#include "Character.h"
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
	chargeBar->setScaleX(0);
	chargeBarBack->runAction(FadeIn::create(0.2f));

	switch (direction)
	{
	case LEFT:	direction = DOWN;  break;
	case RIGHT: direction = UP;  break;
	case DOWN:	direction = RIGHT; break;
	case UP:	direction = LEFT;  break;
	default:
		break;
	}

	return true;
}

void Character::update(float delta)
{
	if (chargeBar->getScaleX() < 1)
	{
		chargeBar->setScaleX(chargeBar->getScaleX() + 0.05f);
		charge = false;
	}
	else
	{

	}

	switch (direction)
	{
	case LEFT:
		setPositionX(getPositionX() - 2);
		break;
	case RIGHT:
		setPositionX(getPositionX() + 2);
		break;
	case DOWN:
		setPositionY(getPositionY() - 2);
		break;
	case UP:
		setPositionY(getPositionY() + 2);
		break;
	default:
		break;
	}
}