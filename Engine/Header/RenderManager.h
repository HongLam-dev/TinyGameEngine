#pragma once
#include <vector>
#include "Window.h"
#include "Camera.h"
#include "SpriteRenderer.h"
#include "UIRenderable.h"
#include "IComponentObserver.h"

namespace TinyEngine {
	class RenderManager:public IComponentObserver
	{
	public:
		void Render(Window& window, const Camera& camera);
		void OnComponentAdded(Component& component);
		void OnComponentRemoved(Component& component);
	private:
		std::vector<UIRenderable*> uiRenderables;
		std::vector<SpriteRenderer*> spriteRenderers;
	};
}