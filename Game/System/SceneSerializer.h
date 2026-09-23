#pragma once
#include "TinyGameEngine.h"
#include "Window.h"
#include <iostream>
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "BoxCollider2D.h"
#include "RigidBody2D.h"
#include "EngineSettings.h"
#include "Animation.h"
#include "Animator.h"
#include "Camera.h"
#include "Image.h"
#include "UIObject.h"
#include "TextureManager.h"
#include "GameComponentRegister.h"
#include <string>

#include "FieldType.h"
namespace TinyGame {
	class SceneSerializer
	{
	public:
		SceneSerializer() = delete;
		static TinyEngine::GameObject& CreateAPingPongBox(TinyEngine::Scene& scene,
			TinyEngine::TinyGameEngine& engine,
			const TinyEngine::Vector3& position,
			const TinyEngine::Vector3& size,std::string texture="");

		static void BuildExampleScene1(TinyEngine::TinyGameEngine& engine,
			TinyEngine::Scene& emptyScene
		);
		static void BuildExampleScene2(TinyEngine::TinyGameEngine& engine,
			TinyEngine::Scene& emptyScene
		);

		static std::string FieldTypeToString(TinyEngine::FieldType type);

		static void SaveScene(TinyEngine::Scene& scene,TinyEngine::ComponentRegister& componentRegister);
	private:
	};
}