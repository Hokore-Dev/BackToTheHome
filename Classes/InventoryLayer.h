#pragma once
#include "cocos2d.h"
#include "ScrollMenu.h"
#include "ui/CocosGUI.h"
#include "Macro.h"

using namespace cocos2d::ui;
class InventoryLayer : public cocos2d::Layer
{
	class Item : public cocos2d::MenuItemImage
	{
	public:
		bool init(std::string name_,int money_,cocos2d::Vec2 position_);
		CREATE_FUNC_ARGS(Item);
		void updateSelect();
	private:
		void selectItem(cocos2d::Ref* sender,bool clear = false);
		cocos2d::Label * lbMoney;
		cocos2d::Label * lbName;
		cocos2d::MenuItemImage * character;
		cocos2d::Sprite * selected;
		std::string name;
		bool have;
		int  money;
	};
public:
	InventoryLayer();
	~InventoryLayer();

	bool init();
	CREATE_FUNC_ARGS(InventoryLayer);
	void updateMenu();
private:
	cocos2d::Size size;
	cocos2d::ui::ScrollView* characterView;
	std::vector<Item*> *items;
};

