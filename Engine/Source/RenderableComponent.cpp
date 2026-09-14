#include "RenderableComponent.h"
#include "GameObject.h"

namespace TinyEngine {
	RenderableComponent::RenderableComponent(GameObject& owner) :Component(owner) {
		GetOwner().RegisterRenderer(*this);
	}
	void RenderableComponent::OnDestroy() {
		GetOwner().UnregisterRenderer(*this);
	}
}