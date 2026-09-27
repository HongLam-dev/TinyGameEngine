#pragma once
#include "EnumString.h"
#include <vector>
#include <string>
namespace TinyEngine {
	struct EnumStringMap
	{
		std::string name;
		std::vector<EnumString> enums;
	};
}