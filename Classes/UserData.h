#pragma once
#include "Singleton.h"
#include "cocos2d.h"

class UserData : public SingleTon<UserData>
{
public:
	UserData();
	~UserData();

	CC_SYNTHESIZE(bool, tutorial, Tutorial);
	CC_SYNTHESIZE(bool, sound, Sound);
	CC_SYNTHESIZE(int, bestScore, BestScore);
	CC_SYNTHESIZE(int, money, Money);
	CC_SYNTHESIZE(int, backgroundCount, BackgroundCount);
	CC_SYNTHESIZE(std::string, selectCharacter, SelectCharacter);

	void init();
	void CharacterDataInit();
};

