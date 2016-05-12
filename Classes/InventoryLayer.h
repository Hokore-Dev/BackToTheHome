#pragma once
#include "cocos2d.h"
#include "Macro.h"

class InventoryLayer : public cocos2d::Layer
{
public:
	InventoryLayer();
	~InventoryLayer();

	bool init();
	CREATE_FUNC_ARGS(InventoryLayer);


};

