#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "Component.h"
#include "Transform.h"
#include "Vector3.h"
#include "RenderableComponent.h"
namespace TinyEngine {

	class Camera;
	class Window;
	class TinyGameEngine;
	class GameObject {
	public:
		GameObject(TinyGameEngine& engine);
		void Update(float deltaTime);
		void FixedUpdate(float fixedDeltaTime);
		void Start();
		void RemoveComponent(Component* component);
		Transform& GetTransform() { return *transform; }
		void RegisterCollider(BoxCollider2D& collider);
		void UnregisterCollider(BoxCollider2D& collider);
		void RegisterRenderer(RenderableComponent& renderer);
		void UnregisterRenderer(RenderableComponent& renderer);
		void DontDestroyOnload(GameObject& gameObject);
		void Destroy(Component& component);
		void Destroy();
		void OnDestroy();
		template <typename T>
			requires std::derived_from<T, Component>
		T& AddComponent()
		{
			auto component = std::make_unique<T>(*this);

			T& ref = *component;

			components.push_back(std::move(component));
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

			return nullptr;
		}

		template <typename T>
			requires std::derived_from<T, Component>
		std::vector<T*> GetAllComponentsOfType()
		{
			std::vector<T*> result;

			for (auto& component : components)
			{
				if (auto* requiredComponent = dynamic_cast<T*>(component.get()))
				{
					result.push_back(requiredComponent);
				}
			}

			return result;
		}

		std::vector<Component*> GetAllComponents()
		{
			std::vector<Component*> result;

			for (auto& component : components)
			{
				result.push_back(component.get());
			}

			return result;
		}
	protected:
		Transform* transform = nullptr;
	private:
		bool destroyOnLoad = true;
		bool destroyed = false;
		TinyGameEngine& engine;
		std::vector<std::unique_ptr<Component>> components;
	};
}