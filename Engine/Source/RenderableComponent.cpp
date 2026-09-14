#include "RenderableComponent.h"
#include "GameObject.h"

namespace TinyEngine {
	void RenderableComponent::Start() {
		GetOwner().RegisterRenderer(*this);
	}
	void RenderableComponent::OnDestroy() {
		GetOwner().UnregisterRenderer(*this);
	}
}