#include "GameObject.h"
#include "SpriteRenderer.h"
#include "TinyGameEngine.h"
#include "Image.h"
#include "UITransform.h"
#include <iostream>

namespace TinyEngine
{
	GameObject::GameObject(TinyGameEngine& engine, ObjectType type): engine(engine),type(type) {
		if (type == ObjectType::WorldObject)
		{
			transform = &AddComponent<Transform>();
		}
		else {
			transform = &AddComponent<UITransform>();
		}
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
			components.erase(it);
		}
	}

	void GameObject::RegisterRenderer(RenderableComponent& renderer) {
		engine.RegisterRenderer(renderer);
	}
	void GameObject::UnregisterRenderer(RenderableComponent& renderer) {
		engine.UnregisterRenderer(renderer);
	}
	void GameObject::RegisterCollider(BoxCollider2D& collider) {
		engine.RegisterCollider(collider);
	}
	void GameObject::UnregisterCollider(BoxCollider2D& collider) {
		engine.UnregisterCollider(collider);
	}
	void GameObject::Destroy(Component& component) {
		if (&component == transform)
		{
			std::cout << "Please don't destroy transform\n";
			return;
		}

		auto it = std::find_if(
			components.begin(),
			components.end(),
			[&component](const std::unique_ptr<Component>& c)
			{
				return c.get() == &component;
			}
		);

		if (it != components.end())
		{
			component.OnDestroy();
			components.erase(it);
		}
	}

	void  GameObject::Destroy() {
		engine.DestroyGameObject(*this);
	}

	void  GameObject::OnDestroy() {
		for (auto& component : components)
		{
			component->OnDestroy();
		}
	}

	void GameObject::DontDestroyOnload(GameObject& gameObject) {
		if (!destroyOnLoad)
			return;
		destroyOnLoad = false;
		engine.DontDestroyOnload(gameObject);
	}
}