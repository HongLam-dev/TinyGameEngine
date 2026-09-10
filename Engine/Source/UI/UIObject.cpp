#include "UIObject.h"
#include "UITransform.h"

namespace TinyEngine {
	UIObject::UIObject() {
		transform = &AddComponent<UITransform>();
	}

}