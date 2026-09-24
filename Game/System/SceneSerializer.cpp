#include "SceneSerializer.h"
#include "TinyGameEngine.h"
#include "Window.h"
#include <iostream>
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "BoxCollider2D.h"
#include "RigidBody2D.h"
#include "Animation.h"
#include "Animator.h"
#include "Camera.h"
#include "Image.h"
#include "UIObject.h"
#include "PingPong.h"
#include "CameraFollow.h"
#include "PlayerController.h"
#include <fstream>
#include <typeindex>

using namespace TinyEngine;

namespace TGModule {
	void SceneSerializer::BuildExampleScene1(TinyEngine::TinyGameEngine& engine,
		TinyEngine::Scene& emptyScene
	) {
		GameObject& playerRef = emptyScene.CreateSceneObject();

	//	sf::Texture* playerRunSheet=Te.GetTexture(");
		std::string placeHolderTex ="Assets/heart.png";

		SpriteRenderer& renderer =
			playerRef.AddComponent<SpriteRenderer>();

		BoxCollider2D& collider =
			playerRef.AddComponent<BoxCollider2D>();

		collider.SetSize({0.64f,0.64f,0});
		playerRef.AddComponent<TinyGame::PlayerController>();

		Rigidbody2D& rb =
			playerRef.AddComponent<Rigidbody2D>();
		rb.SetGravityScale(0.0f);
		rb.SetCollisionDetectMode(CollisionDetectionMode::Continuous);
		//	collider.SetIsTrigger(true);
		renderer.SetTexture("Assets/PlayerRun.png");

		Transform& playerTranform = *playerRef.GetComponent<Transform>();
		playerTranform.SetPosition({2,3,0});
	//	playerTranform.SetScale({ 0.6f,0.6f,0.6f });

		//player animation
		Animation playerAni("Assets/PlayerRun.png");

		playerAni.AddKey({ {{ 53,38 },{ 100, 300 }},0 });
		playerAni.AddKey({ {{ 167,38 },{ 100, 300 }},0.1f });
		playerAni.AddKey({ {{ 311,38 },{ 120, 300 }},0.2f });
		playerAni.AddKey({ {{ 434,38 },{ 100, 300}},0.3f });
		playerAni.AddKey({ {{ 560,38 },{ 100, 300 }},0.4f });
		playerAni.AddKey({ {{ 675,38 },{ 140, 300 }},0.5f });

		Animator& animator = playerRef.AddComponent<Animator>();
		animator.AddAnimation("Run", std::move(playerAni));
		animator.SetAnimation("Run");
		//end

		//Create Image
		UIObject& imageObj = emptyScene.CreateUIObject();
		Image& image = imageObj.AddComponent<Image>();
		image.SetTexture(placeHolderTex);
		Transform& imageTransform = imageObj.GetTransform();
		imageTransform.SetPosition({ 50, 500, 0 });
		//end image

		//Camera
		GameObject& camObj = emptyScene.CreateMainCamera();
		TinyGame::CameraFollow& camFollow = camObj.AddComponent<TinyGame::CameraFollow>();
		camFollow.SetTarget(playerTranform);

		//end camera
		//boxes
		std::string crateText="Assets/Crate.png";

		//CreateASimpleBox({400,200,0},{500,64,0});
		emptyScene.CreateASimpleBox({ 4,3,0 }, { 10.0f,1.0f,0 }, crateText);
		emptyScene.CreateASimpleBox( { 4,1,0 }, { 0.64f,0.64f,0 }, crateText);
		CreateAPingPongBox(emptyScene, engine, { 4,0,0 }, { 0.64f,0.64f,0 }, crateText);
		//end

	}

	void SceneSerializer::BuildExampleScene2(TinyEngine::TinyGameEngine& engine,
		TinyEngine::Scene& emptyScene
	) {
		std::string placeHolderTex = "Assets/heart.png";
		//Create Image
		UIObject& imageObj = emptyScene.CreateUIObject();
		Image& image = imageObj.AddComponent<Image>();
		image.SetTexture(placeHolderTex);
		Transform& imageTransform = imageObj.GetTransform();
		imageTransform.SetPosition({ 0.5f, 5, 0 });
		//end image
		//Camera
		GameObject& camObj = emptyScene.CreateMainCamera();
		//end camera
		//boxes
		std::string crateText = "Assets/Crate.png";

		//CreateASimpleBox({400,200,0},{500,64,0});
		emptyScene.CreateASimpleBox( {4,3,0 }, { 5.0f,0.64f,0 }, crateText);
		emptyScene.CreateASimpleBox( { 4,1,0 }, { 0.64f,0.64f,0 }, crateText);
	//	CreateAPingPongBox(emptyScene, engine, { 400,200,0 }, { 64,64,0 }, crateText);
	//	CreateAPingPongBox(emptyScene, engine, { 400,600,0 }, { 64,64,0 }, crateText);
		//end

	}

	GameObject& SceneSerializer::CreateAPingPongBox(Scene& scene, TinyGameEngine& engine, const Vector3& position, const Vector3& size, std::string boxTexture) {
		GameObject& boxRef = scene.CreateASimpleBox( position, size);

		TinyGame::PingPongAroundCenter& pingpong = boxRef.AddComponent<TinyGame::PingPongAroundCenter>();
		pingpong.Initialize({ 1,0,0 }, 3);

		boxRef.GetComponent<Transform>()
			->SetPosition({
				position.x,
				position.y,
				0
				});
		return boxRef;
	}


	void SceneSerializer::SaveScene(TinyEngine::Scene& scene) {
		std::filesystem::path path =
			std::filesystem::path("Scene") / (scene.GetName()+".tge");
		std::ofstream file(path);

		ComponentRegister& componentRegister = ComponentRegister::Instance();

		for (auto& object : scene.GetGameObjects())
		{
			file << "-Object- \"" << object->GetName()<<"\"{\n";
			for (auto& component : object->GetAllComponents())
			{
				const ComponentInfo* info = componentRegister.FindComponent(typeid(*component));
				if (info)
				{
					file << "	-Component- \"" << info->name << "\"{\n";
					for (const FieldInfo& field : info->fields)
					{
						file << "		-Field- \"" << field.name << "\"{\n";
						file << "		(" << FieldTypeToString(field.type) << ")(";
						switch (field.type)
						{
						case FieldType::Int:
						{
							file << std::any_cast<int>(field.getValue(*component)) ;
							break;
						}
						case FieldType::Float:
						{
							file  << std::any_cast<float>(field.getValue(*component)) ;
							break;
						}
						case FieldType::String:
						{
							file  << std::any_cast<std::string>(field.getValue(*component)) ;
							break;
						}
						case FieldType::Vector2:
						{
							Vector2 value = std::any_cast<Vector2>(field.getValue(*component));
							file << value.x << ")(" << value.y << ")\n";
							break;
						}
						case FieldType::Vector3:
						{
							Vector3 value = std::any_cast<Vector3>(field.getValue(*component));
							file  << value.x << ")(" << value.y << ")(" << value.z ;
							break;
						}
						case FieldType::Bool:
						{
							bool value = std::any_cast<bool>(field.getValue(*component));
							file << value;
							break;
						}
						case FieldType::IntRect:
						{
							sf::IntRect value = std::any_cast<sf::IntRect>(field.getValue(*component));
							file << value.size.x << ")(" << value.size.y << ")(" << value.position.x << ")(" << value.position.y;
							break;
						}
						default:
							break;
						}
						file << ")\n" << "		}\n";
					}
				}
				file << "	}\n";
			}
			file << "}\n";
		}
	}
	std::string SceneSerializer::FieldTypeToString(
		TinyEngine::FieldType type)
	{
		switch (type)
		{
		case TinyEngine::FieldType::Float:
			return "Float";

		case TinyEngine::FieldType::Vector2:
			return "Vector2";

		case TinyEngine::FieldType::Vector3:
			return "Vector3";

		case TinyEngine::FieldType::Int:
			return "Int";

		case TinyEngine::FieldType::Bool:
			return "Bool";

		case TinyEngine::FieldType::String:
			return "String";

		case TinyEngine::FieldType::IntRect:
			return "IntRect";
		}

		return "Unknown";
	}

	TinyEngine::FieldType SceneSerializer::StringToFieldType(std::string type) {
		if (type == "Float")
			return TinyEngine::FieldType::Float;

		if (type == "Vector2")
			return TinyEngine::FieldType::Vector2;

		if (type == "Vector3")
			return TinyEngine::FieldType::Vector3;

		if (type == "Int")
			return TinyEngine::FieldType::Int;

		if (type == "Bool")
			return TinyEngine::FieldType::Bool;

		if (type == "String")
			return TinyEngine::FieldType::String;

		if (type == "IntRect")
			return TinyEngine::FieldType::IntRect;


		return  TinyEngine::FieldType::Unknown;
	}

	void SceneSerializer::LoadScene(std::string sceneName, 
		TinyEngine::TinyGameEngine& engine, 
		TinyEngine::Scene& emptyScene) {
		std::ifstream file("Scene/"+sceneName+".tge");

		ComponentRegister& componentRegister = ComponentRegister::Instance();
		if (!file)
		{
			std::cout << "Failed to open scene\n";
			return;
		}

		std::string line;
		int processingLine=0;
		int processedLine = -1;

		while (std::getline(file, line))
		{
			if (processedLine > processedLine)
			{
				processingLine++;
				continue;
			}
			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] == '-')
				{
					while (line[i] != '"')
					{
						i++;
					}
					i++;
					std::string objectName="";
					while (line[i] != '"')
					{
						objectName += line[i];
						i++;
					}
					GameObject& object = emptyScene.CreateSceneObject();
					object.SetName(objectName);
				}
				processedLine++;
			}
			processingLine++;

		}
	}

}