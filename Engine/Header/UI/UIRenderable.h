#pragma once
#include "RenderableComponent.h"
#include "Window.h"

namespace TinyEngine {
	class UIRenderable: public RenderableComponent
	{
	public:
		UIRenderable(GameObject& owner):RenderableComponent(owner) {};
		virtual void Render(Window& window)=0;
	private:

	};
}