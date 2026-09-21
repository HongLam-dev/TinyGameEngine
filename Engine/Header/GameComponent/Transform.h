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
        void SetPosition(const Vector3& newPosition);
        void SetScale(const Vector3& newScale);
        void SetRotation(const Vector3& newRotation);
        const Vector3& GetPosition() const {return position; }
        const Vector3& GetScale() const { return scale; }
        const Vector3& GetRotation() const { return rotation; }
    private:
        Vector3 position{};
        Vector3 scale{ 1.0f, 1.0f, 1.0f };
        Vector3 rotation{};
    };
}