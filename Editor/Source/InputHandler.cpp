#include "InputHandler.h"
#include "Vector2.h"
#include "EngineSettings.h"
#include "Camera.h"
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
            TinyEngine::Vector2 objectScreenPosition =
                editorCamera.WorldToScreenPosition (selectedObject->GetTransform().GetPosition(),window.GetSize());

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
    void InputHandler::MoveObject(
        TinyEngine::GameObject& objectToMove,
        sf::Vector2i mouseMovement,
        TinyEngine::Camera& camera)
    {
        TinyEngine::Vector2 worldMovement =
        {
            TinyEngine::PixelsToWorld(mouseMovement.x),
            TinyEngine::PixelsToWorld(mouseMovement.y)
        };

        TinyEngine::Vector3 position =
            objectToMove.GetTransform().GetPosition();

        position.x += worldMovement.x;
        position.y += worldMovement.y;

        objectToMove.GetTransform().SetPosition(position);
    }
}