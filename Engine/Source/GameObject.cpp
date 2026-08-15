#include "GameObject.h"
#include "SpriteRenderer.h"
#include <iostream>

namespace TinyEngine
{
	void GameObject::Update()
	{
		for (auto& component : components)
		{
			component->Update();
		}
	}

	void GameObject::Render(Window& window)
	{
		for (auto& component : components)
		{
			if (auto* renderer = dynamic_cast<SpriteRenderer*>(component.get()))
			{
				renderer->Render(window,transform);
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
}