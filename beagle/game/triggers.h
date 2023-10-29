#pragma once

#include "../vector.h"
#include <string>

struct RectangleTrigger
{
	Rectangle area;
	std::string value;
	std::string id;
};

struct TriggerEvent
{
	std::string value;
	std::string id;
};