#pragma once
#include "cocos2d.h"
#include "Macro.h"

enum DIRECTION { LEFT , RIGHT , DOWN ,UP };
class Character : public cocos2d::Sprite
{
public:
	Character();
	~Character();

	bool rotateCharacter();
	bool init(DIRECTION direction_);
	CREATE_FUNC_ARGS(Character);

	bool isMoney()			 const { return money; }
	bool isCharge()			 const { return charge; }
	bool isAnimation()		 const { return animation; }
	DIRECTION getDirection() const { return direction; }

	void setChoose(bool choose_) { choose = choose_; }
	bool getChoose() const { return choose; }

private:
	void initAnimation();
	void update(float delta) override;

	DIRECTION direction;

	cocos2d::Sprite* chargeBarBack;
	cocos2d::Sprite* chargeBar;

	bool animation;
	bool choose;
	bool charge;
	bool money;
};

