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

	void SceneSerializer::SaveScene(TinyEngine::Scene& scene)
	{
		std::filesystem::path path =
			std::filesystem::path("Scenes") /
			(scene.GetName() + ".tge");

		std::ofstream file(path);

		if (!file)
		{
			std::cout << "Failed to open scene file\n";
			return;
		}

		ComponentRegister& componentRegister =
			ComponentRegister::Instance();

		for (auto& object : scene.GetGameObjects())
		{
			file << "<Object name=\""
				<< object->GetName()
				<< "\">\n";

			for (auto& component : object->GetAllComponents())
			{
				const ComponentInfo* info =
					componentRegister.FindComponent(typeid(*component));

				if (!info)
					continue;

				file << "\t<Component name=\""
					<< info->name
					<<"\" id=\""
					<<component->GetID()
					<< "\">\n";

				for (const FieldInfo& field : info->fields)
				{
					file << "\t\t<Field name=\""
						<< field.name
						<< "\" type=\""
						<< FieldTypeToString(field.type)
						<< "\">\n";

					file << "\t\t\t";

					switch (field.type)
					{
					case FieldType::Int:
					{
						int value =
							std::any_cast<int>(
								field.getValue(*component));

						file << value;
						break;
					}

					case FieldType::Float:
					{
						float value =
							std::any_cast<float>(
								field.getValue(*component));

						file << value;
						break;
					}

					case FieldType::String:
					{
						std::string value =
							std::any_cast<std::string>(
								field.getValue(*component));

						file << value;
						break;
					}

					case FieldType::Vector2:
					{
						Vector2 value =
							std::any_cast<Vector2>(
								field.getValue(*component));

						file << value.x << ' '
							<< value.y;

						break;
					}

					case FieldType::Vector3:
					{
						Vector3 value =
							std::any_cast<Vector3>(
								field.getValue(*component));

						file << value.x << ' '
							<< value.y << ' '
							<< value.z;

						break;
					}

					case FieldType::Bool:
					{
						bool value =
							std::any_cast<bool>(
								field.getValue(*component));

						file << value;
						break;
					}

					case FieldType::IntRect:
					{
						sf::IntRect value =
							std::any_cast<sf::IntRect>(
								field.getValue(*component));

						file << value.position.x << ' '
							<< value.position.y << ' '
							<< value.size.x << ' '
							<< value.size.y;

						break;
					}

					case FieldType::Component:
					{
						Component* component =
							std::any_cast<Component*>(
								field.getValue(*component));
						if (component)
						{
							file << component->GetID();
						}
						else {
							file << "None";
						}

						break;
					}
					}

					file << "\n";
					file << "\t\t</Field>\n";
				}

				file << "\t</Component>\n";
			}

			file << "</Object>\n";
		}
	}

	bool SceneSerializer::LoadScene(
		std::string sceneName,
		TinyEngine::Scene& emptyScene)
	{
		std::filesystem::path path =
			std::filesystem::path("Scenes") /
			(sceneName + ".tge");

		std::ifstream file(path);

		if (!file)
		{
			std::cout << "Failed to open scene\n";
			return false;
		}

		std::string line;

		while (std::getline(file, line))
		{
			if (line.find("<Object") == std::string::npos)
				continue;

			std::string objectName =
				GetAttribute(line, "name");

			TinyEngine::GameObject& object =
				emptyScene.CreateSceneObject();

			object.SetName(objectName);

			LoadObject(file, object);
		}

		for (auto& unresolvedReference : unresolvedReferences)
		{
			uint64_t referencedID = unresolvedReference.first;

			for (auto& reference : unresolvedReference.second)
			{
				reference.second(*reference.first,loadedComponents[referencedID]);
			}
		}
		loadedComponents.clear();
		unresolvedReferences.clear();
		return true;
	}

	void SceneSerializer::LoadObject(
		std::ifstream& file,
		TinyEngine::GameObject& object)
	{
		std::string line;

		while (std::getline(file, line))
		{
			if (line.find("</Object>") != std::string::npos)
				return;

			if (line.find("<Component") == std::string::npos)
				continue;

			std::string componentName =
				GetAttribute(line, "name");

			std::uint64_t id = std::stoull(GetAttribute(line, "id"));

			ComponentInfo* info =
				ComponentRegister::Instance()
				.FindComponent(componentName);

			if (!info)
			{
				std::cout
					<< "No component registered: "
					<< componentName
					<< '\n';

				continue;
			}
			Component* component=nullptr;
			if (componentName == "Transform")
			{
				component = &object.GetTransform();
			}
			else {
				component = &info->create(object);
			}
			component->SetID(id);
			LoadComponent(file, *component);

			loadedComponents[id] = component;
		}
	}

	void SceneSerializer::LoadComponent(
		std::ifstream& file,
		TinyEngine::Component& component)
	{
		ComponentInfo* info =
			ComponentRegister::Instance()
			.FindComponent(typeid(component));

		if (!info)
		{
			std::cout << "Component was not registered\n";
			return;
		}

		std::string line;

		while (std::getline(file, line))
		{
			if (line.find("</Component>") != std::string::npos)
				return;

			if (line.find("<Field") == std::string::npos)
				continue;

			std::string fieldName =
				GetAttribute(line, "name");

			std::string fieldType =
				GetAttribute(line, "type");

			const FieldInfo* fieldInfo = nullptr;

			for (const FieldInfo& field : info->fields)
			{
				if (field.name == fieldName)
				{
					fieldInfo = &field;
					break;
				}
			}

			if (!fieldInfo)
			{
				std::cout
					<< "No field registered: "
					<< fieldName
					<< '\n';

				continue;
			}

			// Read the actual value line.
			if (!std::getline(file, line))
				return;

			LoadField(
				component,
				*fieldInfo,
				line
			);

			// Consume </Field>
			std::getline(file, line);
		}
	}
	void SceneSerializer::LoadField(
		TinyEngine::Component& component,
		const FieldInfo& field,
		const std::string& line)
	{
		std::stringstream stream(line);

		switch (field.type)
		{
		case FieldType::Int:
		{
			int value;

			stream >> value;

			field.setValue(component, value);

			break;
		}

		case FieldType::Float:
		{
			float value;

			stream >> value;

			field.setValue(component, value);

			break;
		}

		case FieldType::Bool:
		{
			bool value;

			stream >> value;

			field.setValue(component, value);

			break;
		}

		case FieldType::String:
		{
			std::string value = line;
			value.erase(
				0,
				value.find_first_not_of(" \t\n\r")
			);
			field.setValue(component, value);

			break;
		}

		case FieldType::Vector2:
		{
			Vector2 value;

			stream >> value.x
				>> value.y;

			field.setValue(component, value);

			break;
		}

		case FieldType::Vector3:
		{
			Vector3 value;

			stream >> value.x
				>> value.y
				>> value.z;

			field.setValue(component, value);

			break;
		}

		case FieldType::IntRect:
		{
			sf::IntRect value;

			stream >> value.position.x
				>> value.position.y
				>> value.size.x
				>> value.size.y;

			field.setValue(component, value);

			break;
		}
		case FieldType::Component:
		{
			uint64_t referencedID;
			stream >> referencedID;
			unresolvedReferences[referencedID].push_back({
	&component,
	field.setValue
				});

			break;
		}
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

	std::string SceneSerializer::GetAttribute(
		const std::string& line,
		const std::string& attribute)
	{
		std::string key = attribute + "=\"";

		size_t start = line.find(key);

		if (start == std::string::npos)
			return "";

		start += key.length();

		size_t end = line.find('"', start);

		if (end == std::string::npos)
			return "";

		return line.substr(start, end - start);
	}

}