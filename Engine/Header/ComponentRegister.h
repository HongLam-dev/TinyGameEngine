#pragma once 
#include <typeindex>
#include "ComponentInfo.h"
#include "EnumStringMap.h"

namespace TinyEngine {
	class ComponentRegister
	{
	public:
		template<typename T>
		void RegisterComponent(ComponentInfo info)
		{
			componentInfo[typeid(T)] = std::move(info);
		}

		template<typename T>
		void RegisterEnumClass(EnumStringMap enumMap)
		{
			enumMaps[typeid(T)] = std::move(enumMap);
		}

		const std::unordered_map<std::type_index, ComponentInfo>& GetComponentList() { return componentInfo; };
		const std::unordered_map<std::type_index, EnumStringMap>& GetEnumList() { return enumMaps; };

		EnumStringMap* FindEnumClass(std::type_index typeIndex) {
			if (!enumMaps.contains(typeIndex))
				return nullptr;
			return &enumMaps[typeIndex];
		};

		EnumStringMap* FindEnumClass(std::string enumClassName) {
			for (auto& enumClass : enumMaps)
			{
				if (enumClass.second.name == enumClassName)
					return &enumClass.second;
			}
			return nullptr;
		};


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
		std::unordered_map<std::type_index, EnumStringMap> enumMaps;
	};
}