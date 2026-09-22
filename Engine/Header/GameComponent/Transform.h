#pragma once
#include "Component.h"
#include "Vector3.h"
#include <iostream>

namespace TinyEngine {

    class Transform : public Component
    {
    public:
        Transform(
            GameObject& owner,
            Vector3 position = {},
            Vector3 scale = { 1.0f, 1.0f, 1.0f },
            Vector3 rotation = {}
        ) : position(position), scale(scale), rotation(rotation), Component(owner) {};
        void SetPosition(Vector3 newPosition);
        void SetScale(Vector3 newScale);
        void SetRotation(Vector3 newRotation);
        Vector3 GetPosition() const {return position; }
        Vector3 GetScale() const { return scale; }
        Vector3 GetRotation() const { return rotation; }
    private:
        Vector3 position{};
        Vector3 scale{ 1.0f, 1.0f, 1.0f };
        Vector3 rotation{};
    };
}