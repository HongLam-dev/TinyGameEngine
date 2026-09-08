#include "RenderManager.h"
#include "RenderableComponent.h"
#include "SpriteRenderer.h"
namespace TinyEngine {
	void RenderManager::OnComponentAdded(Component& component) {

		if (auto* uiRenderable = dynamic_cast<UIRenderable*>(&component))
		{
			uiRenderables.push_back(uiRenderable);
		}
		else if (auto* spriteRenderer = dynamic_cast<SpriteRenderer*>(&component))
		{
			spriteRenderers.push_back(spriteRenderer);
		}
	}
	void RenderManager::OnComponentRemoved(Component& component) {

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