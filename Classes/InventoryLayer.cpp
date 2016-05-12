#include "InventoryLayer.h"
USING_NS_CC;

InventoryLayer::InventoryLayer()
{
}

InventoryLayer::~InventoryLayer()
{
}

bool InventoryLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto background = Sprite::create("Texture/storeback.png");
	background->setAnchorPoint(Vec2(0, 0));
	this->addChild(background);

	return true;
}