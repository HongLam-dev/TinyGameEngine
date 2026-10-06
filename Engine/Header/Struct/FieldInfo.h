#pragma once
#include "Component.h"
#include "FieldType.h"
#include <functional>
#include <string>
#include <any>
namespace TinyEngine {
    struct FieldInfo
    {
        std::string name;
        FieldType type;
        std::type_index valueTypeID = typeid(void);
        std::function<std::any(const Component&)> getValue;
        std::function<bool(Component&, const std::any&)> setValue;
        std::function<Component* (const std::any&)> getReferencedComponent;
    };
}