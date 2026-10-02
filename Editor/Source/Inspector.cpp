#include "Inspector.h"
#include <imgui.h>
#include <imgui-SFML.h>
using namespace TinyEngine;

namespace TinyEditor {
    void Inspector::Draw(GameObject* selectedObject)
    {
        ImGui::SetNextWindowSize(
            ImVec2(300, 1280),
            ImGuiCond_FirstUseEver
        );

        BeginWindow("Inspector");
        if (selectedObject)
        {
            DrawSelectedObject(*selectedObject);
            DrawAddComponentMenu(*selectedObject);
        }

        EndWindow();
    }


    void Inspector::DrawSelectedObject(GameObject& gameObject)
    {
        ImGui::Text("%s", gameObject.GetName().c_str());
        for (const auto& component : gameObject.GetAllComponents())
        {
            DrawComponent(*component);
        }
    }


    void Inspector::DrawComponent(Component& component)
    {
        std::type_index type = typeid(component);

        const ComponentInfo* info =
            componentRegister.FindComponent(type);

        if (!info)
            return;

        ImGui::PushID(&component);

        bool open = ImGui::CollapsingHeader(
            info->name.c_str(),
            ImGuiTreeNodeFlags_DefaultOpen
        );


        if (open)
        {
            for (const auto& field : info->fields)
            {
                DrawField(component, field);
            }
            if (ImGui::SmallButton("X"))
            {
                component.Destroy(component);
            }
        }

        ImGui::PopID();
    }

    void Inspector::DrawField(
        Component& component,
        const FieldInfo& field)
    {
        std::any value = field.getValue(component);
        bool isFieldInteracted = false;
        switch (field.type)
        {
        case FieldType::Float:
            isFieldInteracted = DrawFloatField(component, field, value);
            break;

        case FieldType::Vector2:
            isFieldInteracted = DrawVector2Field(component, field, value);
            break;

        case FieldType::Vector3:
            isFieldInteracted = DrawVector3Field(component, field, value);
            break;

        case FieldType::Int:
            isFieldInteracted = DrawIntField(component, field, value);
            break;

        case FieldType::Bool:
            isFieldInteracted = DrawBoolField(component, field, value);
            break;

        case FieldType::String:
            isFieldInteracted = DrawStringField(component, field, value);
            break;

        case FieldType::IntRect:
            isFieldInteracted = DrawIntRectField(component, field, value);
            break;
        case FieldType::Enum:
            isFieldInteracted = DrawEnumField(component, field, value);
            break;
        }
        if (isFieldInteracted)
        {
         //   workingWindow = WorkingWindow::Other;
        }
    }


    bool Inspector::DrawFloatField(
        Component& component,
        const FieldInfo& field,
        const std::any& value)
    {
        float valueFloat =
            std::any_cast<float>(value);

        if (ImGui::DragFloat(
            field.name.c_str(),
            &valueFloat))
        {
            field.setValue(component, valueFloat);
            return true;
        }
        return false;
    }


    bool Inspector::DrawVector2Field(
        Component& component,
        const FieldInfo& field,
        const std::any& value)
    {
        Vector2 valueVector =
            std::any_cast<Vector2>(value);

        if (ImGui::DragFloat2(
            field.name.c_str(),
            &valueVector.x))
        {
            field.setValue(component, valueVector);
            return true;
        }
        return false;
    }


    bool Inspector::DrawVector3Field(
        Component& component,
        const FieldInfo& field,
        const std::any& value)
    {
        Vector3 valueVector =
            std::any_cast<Vector3>(value);

        if (ImGui::DragFloat3(
            field.name.c_str(),
            &valueVector.x))
        {
            field.setValue(component, valueVector);
            return true;
        }
        return false;
    }


    bool Inspector::DrawIntField(
        Component& component,
        const FieldInfo& field,
        const std::any& value)
    {
        int valueInt =
            std::any_cast<int>(value);

        if (ImGui::DragInt(
            field.name.c_str(),
            &valueInt))
        {
            field.setValue(component, valueInt);
            return true;
        }
        return false;
    }


    bool Inspector::DrawBoolField(
        Component& component,
        const FieldInfo& field,
        const std::any& value)
    {
        bool valueBool =
            std::any_cast<bool>(value);

        if (ImGui::Checkbox(
            field.name.c_str(),
            &valueBool))
        {
            field.setValue(component, valueBool);
            return true;
        }
        return false;
    }


    bool Inspector::DrawStringField(
        Component& component,
        const FieldInfo& field,
        const std::any& value)
    {
        std::string valueString =
            std::any_cast<std::string>(value);

        if (editingField != &field ||
            editingComponent != &component)
        {
            std::strncpy(
                fieldStringBuffer,
                valueString.c_str(),
                sizeof(fieldStringBuffer) - 1
            );

            fieldStringBuffer[
                sizeof(fieldStringBuffer) - 1
            ] = '\0';

            editingField = &field;
            editingComponent = &component;
        }

        if (ImGui::InputText(
            field.name.c_str(),
            fieldStringBuffer,
            sizeof(fieldStringBuffer),
            ImGuiInputTextFlags_EnterReturnsTrue))
        {
            field.setValue(
                component,
                std::string(fieldStringBuffer)
            );

            editingField = nullptr;
            editingComponent = nullptr;
        }

        if (ImGui::IsItemDeactivated())
        {
            field.setValue(
                component,
                std::string(fieldStringBuffer)
            );

            editingField = nullptr;
            editingComponent = nullptr;

        }
        return false;
    }


    bool Inspector::DrawIntRectField(
        Component& component,
        const FieldInfo& field,
        const std::any& value)
    {
        sf::IntRect rect =
            std::any_cast<sf::IntRect>(value);

        int values[4] =
        {
            rect.position.x,
            rect.position.y,
            rect.size.x,
            rect.size.y
        };

        if (ImGui::DragInt4(
            field.name.c_str(),
            values))
        {
            rect.position.x = values[0];
            rect.position.y = values[1];
            rect.size.x = values[2];
            rect.size.y = values[3];

            field.setValue(component, rect);
            return true;
        }
        return false;
    }

    bool Inspector::DrawEnumField(
        TinyEngine::Component& component,
        const TinyEngine::FieldInfo& field,
        const std::any& mode)
    {
        EnumStringMap* enumClass =
            componentRegister.FindEnumClass(field.enumType);

        if (!enumClass)
            return false;

        int current = std::any_cast<int>(mode);

        const char* currentName = "Unknown";

        for (const auto& enumValue : enumClass->enums)
        {
            if (enumValue.value == current)
            {
                currentName = enumValue.name.c_str();
                break;
            }
        }

        if (ImGui::BeginCombo(
            field.name.c_str(),
            currentName))
        {
            for (const auto& enumValue : enumClass->enums)
            {
                bool selected =
                    enumValue.value == current;

                if (ImGui::Selectable(
                    enumValue.name.c_str(),
                    selected))
                {
                    field.setValue(
                        component,
                        enumValue.value
                    );
                }

                if (selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        return true;
    }

    void Inspector::DrawAddComponentMenu(
        GameObject& gameObject)
    {
        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("AddComponent");
        }

        if (!ImGui::BeginPopup("AddComponent"))
            return;

        for (const auto& [type, info] :
            componentRegister.GetComponentList())
        {
            if (ImGui::MenuItem(info.name.c_str()))
            {
                info.create(gameObject);
            }
        }

        ImGui::EndPopup();
    }
}