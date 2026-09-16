#pragma once
#include "Input.h"
#include "GameObject.h"

namespace TinyEditor {
	class InputHandler {
	public:
		enum class InputMode
		{
			Scene,
			Typing
		};
		void HandleSceneInput(TinyEngine::GameObject& editorCamera,float cameraMoveSpeed, float deltaTime);
	private:
		TinyEngine::Input& input = TinyEngine::Input::Get();
	};

}