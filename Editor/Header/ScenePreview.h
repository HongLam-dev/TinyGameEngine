#pragma once
#include "EditorWindow.h"
#include "TinyGameEngine.h"
#include "Scene.h"

namespace TinyEditor {
	class ScenePreview
	{
	public:
		void Draw(TinyEngine::Window& window,
			TinyEngine::TinyGameEngine& engine,
			TinyEngine::Scene* editingScene,
			TinyEngine::Camera& editorCamera, 
			TinyEngine::GameObject*& selectedGameObject);
	private:
		void DrawMarker(TinyEngine::Window& window, sf::Vector2f  pixelPosition);
		void DrawObjectMarker(TinyEngine::Window& window, TinyEngine::GameObject*& selectedObject, TinyEngine::Camera& editorCamera);
	};
}