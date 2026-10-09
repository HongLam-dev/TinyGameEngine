#include "ScenePreview.h"
#include "UITransform.h"
using namespace TinyEngine;

namespace TinyEditor {
	void ScenePreview::Draw(Window& window,
		TinyGameEngine& engine,
		Scene* editingScene,
		Camera& editorCamera,
		GameObject*& selectedGameObject) {

		engine.Render(window, editorCamera);

		DrawObjectMarker(window,selectedGameObject,editorCamera);

	}
    void ScenePreview::DrawObjectMarker(Window& window, 
        GameObject*& selectedObject,
        Camera& editorCamera) {
        if (selectedObject)
        {
            TinyEngine::Transform* transform = &selectedObject->GetTransform();
            Vector3 position = selectedObject->GetTransform().GetPosition();
            Vector2 screenPos;
            if (!dynamic_cast<TinyEngine::UITransform*>(transform))
            {
                screenPos = editorCamera.WorldToScreenPosition(position, window.GetSize());
            }
            else {
                screenPos.x = position.x;
                screenPos.y = position.y;
            }
      
            DrawMarker(window,{ screenPos.x,screenPos.y });
        }
    }
    void ScenePreview::DrawMarker(Window& window, sf::Vector2f pixelPosition)
    {
        sf::Texture* tex = TextureManager::Instance().GetTexture("Assets/Hand.png");
        if (tex)
        {
            sf::Sprite marker(
                *tex
            );

            sf::Vector2u size = marker.getTexture().getSize();

            marker.setOrigin(
                sf::Vector2f(
                    (size.x / 2.0f),
                    (size.y / 2.0f)
                )
            );

            marker.setPosition(pixelPosition);

            window.GetRenderWindow()->draw(marker);
        }
        else
        {
            std::cout << "No texture for marker found\n";
        }

    }
}