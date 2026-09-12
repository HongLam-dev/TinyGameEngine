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

namespace TinyGame {
	class SceneBuilder
	{
	public:
		SceneBuilder() = delete;
		static TinyEngine::GameObject& CreateASimpleBox(TinyEngine::Scene& scene,
			TinyEngine::TinyGameEngine& engine,
			const TinyEngine::Vector3& position, 
			const TinyEngine::Vector3& size, 
			sf::Texture* boxTexture);

		static TinyEngine::GameObject& CreateAPingPongBox(TinyEngine::Scene& scene,
			TinyEngine::TinyGameEngine& engine,
			const TinyEngine::Vector3& position,
			const TinyEngine::Vector3& size,
			sf::Texture* boxTexture);

		static void BuildExampleScene1(TinyEngine::TinyGameEngine& engine,
			TinyEngine::Scene& emptyScene,
			TinyEngine::TextureManager& textureManager
		);
		static void BuildExampleScene2(TinyEngine::TinyGameEngine& engine,
			TinyEngine::Scene& emptyScene,
			TinyEngine::TextureManager& textureManager
		);
	private:
	};
}