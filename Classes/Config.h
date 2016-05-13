#pragma once

#define LEVEL_DELTA			15.0f
#define LEVEL_BALL_RANGE	1

#define BALL_SPEED			2.0f
#define BALL_CREATE_TIME	1.0f
#define BALL_CHARGEBAR_INCREASE_VALUE 0.02f

#define MAP_COUNT 4

#include <string>
static std::string getHideItemString(std::string itemName)
{
	if (itemName == "box") return "box";
	if (itemName == "circle") return "XirXXe";
	if (itemName == "star") return "XtXX";
	if (itemName == "man") return "mXX";
	if (itemName == "butterfly") return "XXttXrXXy";
	if (itemName == "cart") return "XXrt";
	if (itemName == "woman") return "wXoXan";
	if (itemName == "question") return "quXXtXoX";
	if (itemName == "moon") return "mXXn";
	if (itemName == "car") return "XXr";

}