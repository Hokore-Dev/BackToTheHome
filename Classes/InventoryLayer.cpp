#include "InventoryLayer.h"
#include "UserData.h"
#include "Config.h"
#include "SimpleAudioEngine.h"

using CocosDenshion::SimpleAudioEngine;
USING_NS_CC;

InventoryLayer::InventoryLayer()
{
}

InventoryLayer::~InventoryLayer()
{
	items->clear();
	delete items;
}

bool InventoryLayer::Item::init(std::string name_,int money_,Vec2 position_)
{
	if (!initWithNormalImage("Texture/storeMenulock.png",
		"Texture/storeMenulock.png","Texture/storeMenulock.png",
		std::bind([&]{ selectItem(this, false); })))
	{
		return false;
	}
	have = false;
	name = name_;
	money = money_;
	this->setPosition(position_);

	lbMoney = Label::create(StringUtils::toString(money_),
		"fonts/calibri.ttf", 50);
	lbMoney->setPosition(60, 64);
	this->addChild(lbMoney);

	lbName = Label::create(getHideItemString(name_),
		"fonts/calibri.ttf", 40);
	lbName->setPosition(getContentSize().width/2, 10);
	this->addChild(lbName);

	Menu* menu = Menu::create();
	menu->setPosition(Vec2::ZERO);

	std::string texture = StringUtils::format(
		"Texture/%s/%d.png", name.c_str(), cocos2d::random(0, 2));

	character = MenuItemImage::create(texture, texture,
		std::bind([&]{ selectItem(this, false); }));
	character->setVisible(false);
	character->setScale(1.3f);
	character->setPosition(getContentSize().width / 2, 120);
	menu->addChild(character);

	this->addChild(menu);

	selected = Sprite::create("Texture/selectedMenu.png");
	selected->setVisible(false);
	selected->setAnchorPoint(Vec2(0, 0));
	this->addChild(selected);

	if (UserDefault::getInstance()->getBoolForKey(name.c_str()))
	{
		have = true;
		selectItem(this,true);
	}

	return true;
}
void InventoryLayer::Item::updateSelect()
{
	if (name == UserDefault::getInstance()->getStringForKey("selectCharacter"))
	{
		selected->setVisible(true);
	}
	else
	{
		selected->setVisible(false);
	}
}

void InventoryLayer::Item::selectItem(cocos2d::Ref* sender,bool clear)
{
	if (have)
	{
		lbMoney->setVisible(false);
		lbName->setString(name);
		this->setSelectedImage(Sprite::create("Texture/storeMenu.png"));
		this->setNormalImage(Sprite::create("Texture/storeMenu.png"));
		this->getSelectedImage()->setZOrder(-1);
		this->getNormalImage()->setZOrder(-1);
		character->setVisible(true);

		if (!clear)
		{
			SimpleAudioEngine::getInstance()->playEffect("Sound/touch.wav");
			UserDefault::getInstance()->setStringForKey("selectCharacter", name);
			UserData::getInstance()->setSelectCharacter(name);
			dynamic_cast<InventoryLayer*>(this->getParent()->getParent()->getParent()->getParent())->updateMenu();
		}
	}
	else if (!clear)
	{
		if (UserData::getInstance()->getMoney() >= money)
		{
			SimpleAudioEngine::getInstance()->playEffect("Sound/touch.wav");
			UserData::getInstance()->setMoney(UserData::getInstance()->getMoney() - money);
			UserDefault::getInstance()->setIntegerForKey("Money", UserData::getInstance()->getMoney());
			UserDefault::getInstance()->setBoolForKey(name.c_str(), true);
			UserDefault::getInstance()->setStringForKey("selectCharacter", name);
			UserData::getInstance()->setSelectCharacter(name);

			lbMoney->setVisible(false);
			lbName->setString(name);
			this->setSelectedImage(Sprite::create("Texture/storeMenu.png"));
			this->setNormalImage(Sprite::create("Texture/storeMenu.png"));
			this->getSelectedImage()->setZOrder(-1);
			this->getNormalImage()->setZOrder(-1);
			character->setVisible(true);
			have = true;

			UserDefault::getInstance()->setStringForKey("selectCharacter", name);
			UserData::getInstance()->setSelectCharacter(name);
			dynamic_cast<InventoryLayer*>(this->getParent()->getParent()->getParent()->getParent())->updateMenu();
		}
		else
		{
			SimpleAudioEngine::getInstance()->playEffect("Sound/cancle.wav");
		}
	}
}

bool InventoryLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	items = new std::vector<Item*>;
	size = Director::getInstance()->getWinSize();

	Menu * groundMenu = Menu::create();
	groundMenu->setPosition(Vec2::ZERO);
	this->addChild(groundMenu);

	auto background = MenuItemImage::create("Texture/storeback.png",
		"Texture/storeback.png", std::bind([&]{}));
	background->setAnchorPoint(Vec2(0, 0));
	groundMenu->addChild(background);

	characterView = ScrollView::create();
	characterView->setDirection(ListView::Direction::VERTICAL);
	characterView->setSize(Size(550, 1280));
	characterView->setContentSize(Size(550, 900));
	characterView->setScrollBarPositionFromCorner(Vec2(15, 0));
	characterView->setBounceEnabled(true);
	characterView->setPosition(Vec2(100,200));
	this->addChild(characterView);
	
	Menu* backMenu = Menu::create();
	backMenu->setPosition(Vec2::ZERO);
	auto back = MenuItemImage::create("Texture/back.png", "Texture/back.png",
		std::bind([&]{
		SimpleAudioEngine::getInstance()->playEffect("Sound/touch.wav");
		this->removeFromParentAndCleanup(true);
	}));
	back->setPosition(size.width / 2, 100);
	backMenu->addChild(back);
	this->addChild(backMenu);

	auto menu = ScrollMenu::create(nullptr);
	menu->setPosition(Vec2::ZERO);
	characterView->addChild(menu);

	Item* m[10];
	m[0] = Item::create("box", 0, Vec2(150, 1100 - 240 * 0));
	m[1] = Item::create("circle", 10, Vec2(400, 1100 - 240 * 0));
	m[2] = Item::create("star", 10, Vec2(150, 1100 - 240 * 1));
	m[3] = Item::create("man", 15, Vec2(400, 1100 - 240 * 1));
	m[4] = Item::create("butterfly", 15, Vec2(150, 1100 - 240 * 2));
	m[5] = Item::create("cart", 20, Vec2(400, 1100 - 240 * 2));
	m[6] = Item::create("woman", 15, Vec2(150, 1100 - 240 * 3));
	m[7] = Item::create("question", 15, Vec2(400, 1100 - 240 * 3));
	m[8] = Item::create("moon", 15, Vec2(150, 1100 - 240 * 4));
	m[9] = Item::create("car", 20, Vec2(400, 1100 - 240 * 4));
	for (int i = 0; i < 10; i++)
	{
		menu->addChild(m[i]);
		items->push_back(m[i]);
	}
	updateMenu();
	return true;
}

void InventoryLayer::updateMenu()
{
	for (Item* v : *items)
		v->updateSelect();
}