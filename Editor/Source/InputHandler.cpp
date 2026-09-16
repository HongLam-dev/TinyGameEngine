#include "InputHandler.h"
#include <SFML/Window.hpp>

namespace TinyEditor {
	void InputHandler::HandleSceneInput(TinyEngine::GameObject& editorCamera, float cameraMoveSpeed, float deltaTime) {

		if (input.IsMousePressed(sf::Mouse::Button::Left))
		{
			sf::Vector2i Direction = input.GetMouseMovement();
		
			TinyEngine::Vector3 cameraPosition = editorCamera.GetTransform().GetPosition();
			cameraPosition.x = cameraPosition.x+cameraMoveSpeed * Direction.x*deltaTime;
			cameraPosition.y = cameraPosition.y + cameraMoveSpeed * Direction.y * deltaTime;
			editorCamera.GetTransform().SetPosition(cameraPosition);
		}
	}
}