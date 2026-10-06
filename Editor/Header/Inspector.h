#pragma once
#include "ComponentRegister.h"
#include "Scene.h"
#include "EditorWindow.h"
namespace TinyEditor {
    class Inspector:public EditorWindow
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

        void DrawFloatField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        void DrawVector2Field(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        void DrawVector3Field(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        void DrawIntField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        void DrawBoolField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        void DrawStringField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        void DrawIntRectField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        void DrawEnumField(TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value);
        void DrawComponentField(TinyEngine::Component& component,
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