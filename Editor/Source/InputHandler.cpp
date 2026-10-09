#include "InputHandler.h"
#include "Vector2.h"
#include "EngineSettings.h"
#include "Camera.h"
#include "GameObject.h"
#include <SFML/Window.hpp>

namespace TinyEditor {
    void InputHandler::HandleSceneInput(
        TinyEngine::Camera& editorCamera,
        TinyEngine::GameObject*& selectedObject,
        float deltaTime)
    {
        if (!input.IsMousePressed(sf::Mouse::Button::Left))
            return;

        sf::Vector2i mouseMovement = input.GetMouseMovement();

        if (selectedObject)
        {
            TinyEngine::Transform* transform = &selectedObject->GetTransform();
            TinyEngine::Vector2 objectScreenPosition;
            if (selectedObject->GetType() == TinyEngine::ObjectType::WorldObject)
            {
                objectScreenPosition = editorCamera.WorldToScreenPosition(transform->GetPosition(), window.GetSize());
            }
            else {
                objectScreenPosition.x = transform->GetPosition().x;
                objectScreenPosition.y = transform->GetPosition().y;
            }

            TinyEngine::Vector2 mousePosition =
                input.GetMousePosition(window.GetPosition());

            float distance =
                TinyEngine::Vector2::Distance(objectScreenPosition, mousePosition);
            if (distance < 50)
            {
                MoveObject(*selectedObject, -mouseMovement, editorCamera);
                return;
            }
        }
        MoveObject(editorCamera.GetOwner(), mouseMovement, editorCamera);
    }
    void InputHandler::HandleGlobalInput() {
        if (input.isKeyPressed(sf::Keyboard::Key::LControl))
        {
            if (input.OnKeyDown(sf::Keyboard::Key::S))
            {
                saveSceneCallback();
            }
            else if(input.OnKeyDown(sf::Keyboard::Key::D)) {
                duplicateCallback();
            }
        }
    }
    void InputHandler::MoveObject(
        TinyEngine::GameObject& objectToMove,
        sf::Vector2i mouseMovement,
        TinyEngine::Camera& camera)
    {
        TinyEngine::Vector3 position =
            objectToMove.GetTransform().GetPosition();
        if (objectToMove.GetType() == TinyEngine::ObjectType::WorldObject)
        {
            TinyEngine::Vector2 worldMovement=
            {
                TinyEngine::PixelsToWorld(mouseMovement.x),
                TinyEngine::PixelsToWorld(mouseMovement.y)
            };

            position.x += worldMovement.x;
            position.y += worldMovement.y;
        }
        else {
            position.x += mouseMovement.x;
            position.y += mouseMovement.y;
        }

        objectToMove.GetTransform().SetPosition(position);
    }
}