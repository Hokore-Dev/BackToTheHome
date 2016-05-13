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
	UserDefault::getInstance()->setBoolForKey("circle", false);
	UserDefault::getInstance()->setBoolForKey("star", false);
	UserDefault::getInstance()->setBoolForKey("man", false);
	UserDefault::getInstance()->setBoolForKey("butterfly", false);
	UserDefault::getInstance()->setBoolForKey("cart", false);
	UserDefault::getInstance()->setBoolForKey("woman", false);
	UserDefault::getInstance()->setBoolForKey("question", false);
	UserDefault::getInstance()->setBoolForKey("moon", false);
	UserDefault::getInstance()->setBoolForKey("car", false);
	UserDefault::getInstance()->setStringForKey("selectCharacter", "box");

}