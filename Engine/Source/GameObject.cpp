#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Image.h"
#include <iostream>

namespace TinyEngine
{
	GameObject::GameObject() {
		transform = &AddComponent<Transform>();
	}
	void GameObject::Update(float deltaTime)
	{
		for (auto& component : components)
		{
			component->Update(deltaTime);
		}
	}

	void GameObject::FixedUpdate(float fixedDeltaTime)
	{
		for (auto& component : components)
		{
			component->FixedUpdate(fixedDeltaTime);
		}
	}

	void GameObject::Start()
	{
		for (auto& component : components)
		{
			component->Start();
		}
	}


	void GameObject::NotifyComponentAdded(Component& component)
	{
		for (IComponentObserver* observer : observers)
		{
			observer->OnComponentAdded(component);
		}
	}

	void GameObject::NotifyComponentRemoved(Component& component)
	{
		for (IComponentObserver* observer : observers)
		{
			observer->OnComponentRemoved(component);
		}
	}

	void GameObject::AddComponentObserver(IComponentObserver& observer)
	{
		observers.push_back(&observer);
	}

	void GameObject::RemoveComponentObserver(IComponentObserver& observer)
	{
		std::erase(observers, &observer);
	}

	void GameObject::RemoveComponent(Component* component)
	{
		auto it = std::find_if(
			components.begin(),
			components.end(),
			[component](const std::unique_ptr<Component>& ptr)
			{
				return ptr.get() == component;
			});

		if (it != components.end())
		{
			NotifyComponentRemoved(**it);

			components.erase(it);
		}
	}
}