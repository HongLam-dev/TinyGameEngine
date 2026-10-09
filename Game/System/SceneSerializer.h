#pragma once
#include "TinyGameEngine.h"
#include "Window.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "BoxCollider2D.h"
#include "RigidBody2D.h"
#include "EngineSettings.h"
#include "Animation.h"
#include "Animator.h"
#include "Camera.h"
#include "Image.h"
#include "TextureManager.h"
#include "GameComponentRegister.h"
#include "FieldType.h"
#include <string>
#include <iostream>
namespace TGModule {
	class SceneSerializer
	{
	public:
		std::string FieldTypeToString(TinyEngine::FieldType type);
		void SaveScene(TinyEngine::Scene& scene, std::string pathToSave);
		bool LoadScene(std::string sceneName,
			TinyEngine::Scene& emptyScene);
		static bool FindScene(std::string scenePath);

		static SceneSerializer& Instance() {
			static SceneSerializer serializer;
			return serializer;
		}
	private:
		SceneSerializer() {};
		std::string GetAttribute(
			const std::string& line,
			const std::string& attribute);

		void LoadObject(
			std::ifstream& file,
			TinyEngine::GameObject& object);

		void LoadComponent(
			std::ifstream& file,
			TinyEngine::Component& component);

		void LoadField(
			TinyEngine::Component& component,
			const TinyEngine::FieldInfo& field,
			const std::string& line);
		std::unordered_map<
			std::uint64_t,
			std::vector<
			std::pair<
			TinyEngine::Component*,
			std::function<void(
				TinyEngine::Component&,
				const std::any&
				)>
			>
			>
		> unresolvedReferences;

		std::unordered_map< std::uint64_t, TinyEngine::Component*> loadedComponents;
	};
}