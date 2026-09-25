#pragma once 
#include <typeindex>
#include "ComponentInfo.h"

namespace TinyEngine {
	class ComponentRegister
	{
	public:
		template<typename T>
		void RegisterComponent(ComponentInfo info)
		{
			componentInfo[typeid(T)] = std::move(info);
		}
		const std::unordered_map<std::type_index, ComponentInfo>& GetComponentList() { return componentInfo; };
		ComponentInfo* FindComponent(std::type_index typeIndex) { 
			if (!componentInfo.contains(typeIndex))
				return nullptr;
			return &componentInfo[typeIndex]; 
		};

		ComponentInfo* FindComponent(std::string componentName) {
			for (auto& component : componentInfo)
			{
				if (component.second.name == componentName)
					return &component.second;
			}
			return nullptr;
		};
		static ComponentRegister& Instance()
		{
			static ComponentRegister componentRegister;
			return componentRegister;
		}
	private:
		std::unordered_map<std::type_index, ComponentInfo> componentInfo;
	};
}