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
        std::function<std::any(const Component&)> getValue;
        std::function<void(Component&, const std::any&)> setValue;
    };
}