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
	cocos2d::Size size;

	cocos2d::Label  * lbScore;
	cocos2d::Sprite * crashRect;
	cocos2d::Sprite * arrow;
	cocos2d::Sprite * home;

	Character* targetCharacter;
	std::vector<Character*> *characters;

	float	levelDelta;
	float	ballCreateDelta;
	int		levelBallCount;
	int		level;
	int		score;
};

#endif
