#pragma once
#include "Window.h"
#include "Input.h"
#include "Vector2.h"
#include "GameObject.h"
#include <functional>

namespace TinyEditor {
	class InputHandler {
	public:
		InputHandler(TinyEngine::Window& window, std::function<void()> saveSceneCallback, std::function<void()> loadSceneCallback)
			: window(window),saveSceneCallback(saveSceneCallback),loadSceneCallback(loadSceneCallback) {};
		void HandleSceneInput(TinyEngine::Camera& editorCamera, TinyEngine::GameObject*& selectedObject , float deltaTime);
		void MoveObject(
			TinyEngine::GameObject& objectToMove,
			sf::Vector2i mouseMovement,
			TinyEngine::Camera& camera);
		void HandleGlobalInput();
	private:
		TinyEngine::Window& window;
		TinyEngine::Vector2 cameraMoveSpeed{5,5};
		TinyEngine::Input& input = TinyEngine::Input::Get();
		std::function<void()> saveSceneCallback;
		std::function<void()> loadSceneCallback;
	};

}