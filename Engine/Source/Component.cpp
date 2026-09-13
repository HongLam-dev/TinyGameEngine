#include "GameObject.h"
namespace TinyEngine
{
	void Component::Destroy(Component& component) {
		component.GetOwner().Destroy(component);
	}
	void Component::Destroy(GameObject& gameObject) {
		gameObject.Destroy();
	}

	const GameObject& Component::GetOwner() const
	{
		return owner;
	}

	GameObject& Component::GetOwner()
	{
		return owner;
	}

	Transform& Component::GetTransform() const {
		return owner.GetTransform();
	}

	void Component::DontDestroyOnload(GameObject& gameObject) {
		GetOwner().DontDestroyOnload(gameObject);
	}
	void Component::DontDestroyOnload(Component& component) {
		GetOwner().DontDestroyOnload(component.GetOwner());
	}
}