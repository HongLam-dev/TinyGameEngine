#pragma once 
#include <vector>
#include "ComponentInfo.h"

namespace TinyEngine {
	class ComponentRegister
	{
	public:
		void RegisterComponent(ComponentInfo info) { componentInfo.push_back(info); };
		const std::vector<ComponentInfo>& GetComponentList() { return componentInfo; };
	private:
		std::vector<ComponentInfo> componentInfo;
	};
}