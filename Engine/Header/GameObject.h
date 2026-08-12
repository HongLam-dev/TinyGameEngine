#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "Component.h"
#include "Transform.h"
#include "Vector3.h"
#include "Window.h"
namespace TinyEngine {

	class GameObject {
	public:
		GameObject() {}
		void Update();
		void FixedUpdate();
		void Render(Window& window);
		const Transform& GetTransform() const
		{
			return transform;
		}
		Transform& GetTransform()
		{
			return transform;
		}
		template <typename T>
			requires std::derived_from<T, Component>
		T& AddComponent()
		{
			auto component = std::make_unique<T>();

			T& ref = *component;

			components.push_back(std::move(component));
			return ref;
		}
	private:
		Transform transform;
		std::vector<std::unique_ptr<Component>> components;
	};
}