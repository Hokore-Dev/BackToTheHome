#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class Character;
class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
	GameScene();
	~GameScene();

    virtual bool init();
	void update(float delta) override;
	CREATE_FUNC(GameScene);

	virtual void onTouchesBegan(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* unused_event) override;
	virtual void onTouchesMoved(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* unused_event) override;
	virtual void onTouchesEnded(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* unused_event) override;

private:
	void dataInitailize();
	void gameResultShow();

	cocos2d::Size size;

	cocos2d::Label  * lbScore;
	cocos2d::Sprite * crashRect;
	cocos2d::Sprite * arrow;
	cocos2d::Sprite * home;
	cocos2d::Sprite * moneyIcon;
	cocos2d::Label  * lbUserMoney;
	cocos2d::Label  * bestScore;
	cocos2d::Sprite * clearMessage;
	cocos2d::Sprite * ranking;
	cocos2d::Sprite * share;
	cocos2d::Sprite * moneyUp;

	Character* targetCharacter;
	std::vector<Character*> *characters;
	std::vector<cocos2d::Sprite*> *moneyItems;

	float	levelDelta;
	float	ballCreateDelta;
	int		levelBallCount;
	int		level;
	int		score;
	int		userMoney;
	bool	gameover;
};

#endif
