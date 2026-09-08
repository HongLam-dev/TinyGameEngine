#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Image.h"
#include <iostream>

namespace TinyEngine
{
	GameObject::GameObject(TinyGameEngine& engine) : engine(engine) {
		transform = &AddComponent<Transform>();
	}
	void GameObject::Update()
	{
		for (auto& component : components)
		{
			component->Update();
		}
	}

	void GameObject::Render(Window& window,const Camera& camera)
	{
		for (auto& component : components)
		{
			if (auto* renderer = dynamic_cast<SpriteRenderer*>(component.get()))
			{
				renderer->Render(window,camera);
			}
			else if(auto* image = dynamic_cast<Image*>(component.get()))
			{
				image->Render(window);
			}
		}
	}

	void GameObject::FixedUpdate()
	{
		for (auto& component : components)
		{
			component->FixedUpdate();
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