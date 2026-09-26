#include "RenderableComponent.h"
#include "GameObject.h"

namespace TinyEngine {
	RenderableComponent::RenderableComponent(GameObject& owner) :Component(owner) {
		GetOwner().RegisterRenderer(*this);
	}
	void RenderableComponent::OnDestroy() {
		Component::OnDestroy();
		GetOwner().UnregisterRenderer(*this);
	}
}