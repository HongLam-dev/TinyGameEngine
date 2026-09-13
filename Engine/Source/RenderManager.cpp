#include "RenderManager.h"
#include "RenderableComponent.h"
#include "SpriteRenderer.h"
namespace TinyEngine {
	void RenderManager::RegisterRenderer(RenderableComponent& renderer) {
		if (auto* uiRenderable = dynamic_cast<UIRenderable*>(&renderer))
		{
			uiRenderables.push_back(uiRenderable);
		}
		else if (auto* spriteRenderer = dynamic_cast<SpriteRenderer*>(&renderer))
		{
			spriteRenderers.push_back(spriteRenderer);
		}
	}
	void  RenderManager::UnregisterRenderer(RenderableComponent& renderer) {
		if (auto* uiRenderable = dynamic_cast<UIRenderable*>(&renderer))
		{
			auto it = std::find(uiRenderables.begin(), uiRenderables.end(), &renderer);
			if (it != uiRenderables.end())
			{
				uiRenderables.erase(it);
			}
		}
		else if (auto* spriteRenderer = dynamic_cast<SpriteRenderer*>(&renderer))
		{
			auto it = std::find(spriteRenderers.begin(), spriteRenderers.end(), &renderer);
			if (it != spriteRenderers.end())
			{
				spriteRenderers.erase(it);
			}
		}
	}

	void RenderManager::Render(Window& window, const Camera& camera) {
		for (auto& renderer : spriteRenderers)
		{
			renderer->Render(window,camera);
		}
		for (auto& uiRenderer : uiRenderables)
		{
			uiRenderer->Render(window);
		}
	}

 }