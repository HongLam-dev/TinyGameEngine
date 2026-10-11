#include "Button.h"
#include "Input.h"

namespace TinyEngine{
	void Button::Invoke() {
		for (auto& callback : callbacks)
		{
			callback();
		}
	}

	void  Button::Start() {
		if (!targetGraphic)
		{
			GetOwner().GetComponent<Image>();
		}
	}
	void  Button::Update(float deltaTime) {
		if (!targetGraphic)
			return;
	}
}