#pragma once
#include <vector>
#include "Window.h"
#include "Camera.h"
#include "SpriteRenderer.h"
#include "UIRenderable.h"

namespace TinyEngine {
	class RenderManager
	{
	public:
		void Render(Window& window, const Camera& camera);
		void RegisterRenderer(RenderableComponent& renderer);
		void UnregisterRenderer(RenderableComponent& renderer);
	private:
		std::vector<UIRenderable*> uiRenderables;
		std::vector<SpriteRenderer*> spriteRenderers;
	};
}