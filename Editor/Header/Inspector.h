#pragma once
#include "ComponentRegister.h"
#include "Scene.h"
namespace TinyEditor {
    class Inspector
    {
    public:
        void Draw(
            TinyEngine::GameObject* selectedObject
        );

    private:
        void DrawSelectedObject(TinyEngine::GameObject& gameObject);

        void DrawComponent(TinyEngine::Component& component);

        void DrawField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field
        );

        bool DrawFloatField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        bool DrawVector2Field(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        bool DrawVector3Field(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        bool DrawIntField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        bool DrawBoolField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        bool DrawStringField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        bool DrawIntRectField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        bool DrawEnumField(TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value);

        void DrawAddComponentMenu(TinyEngine::GameObject& gameObject);

        TinyEngine::ComponentRegister& componentRegister =
            TinyEngine::ComponentRegister::Instance();

        char fieldStringBuffer[256]{};

        const TinyEngine::FieldInfo* editingField = nullptr;
        TinyEngine::Component* editingComponent = nullptr;
    };
}