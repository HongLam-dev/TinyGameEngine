#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "Component.h"
#include "Transform.h"
#include "Vector3.h"
#include "Window.h"
#include "IComponentObserver.h"
namespace TinyEngine {

	class TinyGameEngine;
	class GameObject {
	public:
		GameObject(TinyGameEngine& engine) : transform(*this), engine(engine) {}
		void Update();
		void FixedUpdate();
		void Start();
		void Render(Window& window);
		TinyGameEngine& GetEngineContext() const { return engine; };
		void AddComponentObserver(IComponentObserver& observer);
		void RemoveComponentObserver(IComponentObserver& observer);
		void RemoveComponent(Component* component);

		template <typename T>
			requires std::derived_from<T, Component>
		T& AddComponent()
		{
			auto component = std::make_unique<T>(*this);

			T& ref = *component;

			components.push_back(std::move(component));

			NotifyComponentAdded(ref);
			return ref;
		}

		template <typename T>
			requires std::derived_from<T, Component>
		T* GetComponent()
		{
			for (auto& component : components)
			{
				if (auto* requiredComponent = dynamic_cast<T*>(component.get()))
				{
					return requiredComponent;
				}
			}
			if (auto* requiredComponent = dynamic_cast<T*>(&transform))
			{
				return requiredComponent;
			}

			return nullptr;
		}

		template <typename T>
			requires std::derived_from<T, Component>
		const T* GetComponent() const
		{
			for (auto& component : components)
			{
				if (auto* requiredComponent = dynamic_cast<const T*>(component.get()))
				{
					return requiredComponent;
				}
			}
			if (auto* requiredComponent = dynamic_cast<const T*>(&transform))
			{
				return requiredComponent;
			}

			return nullptr;
		}

	private:
		Transform transform;
		std::vector<std::unique_ptr<Component>> components;
		TinyGameEngine& engine;

		void NotifyComponentAdded(Component& component);
		void NotifyComponentRemoved(Component& component);

		std::vector<IComponentObserver*> observers;
	};
}