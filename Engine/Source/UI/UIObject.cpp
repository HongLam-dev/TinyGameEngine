#include "UIObject.h"
#include "UITransform.h"

namespace TinyEngine {
	UIObject::UIObject(TinyGameEngine& engine) :GameObject(engine) {
		transform = &AddComponent<UITransform>();
	}
}