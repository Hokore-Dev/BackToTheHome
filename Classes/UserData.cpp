#include "UserData.h"
USING_NS_CC;

UserData::UserData() : backgroundCount(0), money(0), sound(true)
{
}


UserData::~UserData()
{
}

void UserData::init()
{
	if (!UserDefault::getInstance()->getBoolForKey("Game"))
	{
		UserDefault::getInstance()->setBoolForKey("Game", true);
		UserDefault::getInstance()->setIntegerForKey("Money", 0);
		UserDefault::getInstance()->setBoolForKey("Sound", true);
		UserDefault::getInstance()->setBoolForKey("Tutorial", true);
		UserDefault::getInstance()->setIntegerForKey("BestScore", 0);
		CharacterDataInit();
	}
	bestScore = UserDefault::getInstance()->getIntegerForKey("BestScore");
	money = UserDefault::getInstance()->getIntegerForKey("Money");
	tutorial = UserDefault::getInstance()->getBoolForKey("Tutorial");
	sound = UserDefault::getInstance()->getBoolForKey("Sound");
	selectCharacter = UserDefault::getInstance()->getStringForKey("selectCharacter");
}

void UserData::CharacterDataInit()
{
	UserDefault::getInstance()->setBoolForKey("box", true);
	UserDefault::getInstance()->setBoolForKey("circle", true);
	UserDefault::getInstance()->setBoolForKey("star", true);
	UserDefault::getInstance()->setBoolForKey("man", true);
	UserDefault::getInstance()->setBoolForKey("butterfly", true);
	UserDefault::getInstance()->setBoolForKey("cart", true);
	UserDefault::getInstance()->setBoolForKey("woman", true);
	UserDefault::getInstance()->setBoolForKey("question", true);
	UserDefault::getInstance()->setBoolForKey("moon", true);
	UserDefault::getInstance()->setBoolForKey("car", false);
	UserDefault::getInstance()->setStringForKey("selectCharacter", "box");

}