#pragma once
#include <string>
#include <functional>
#include <vector>
#include "FieldInfo.h"
#include "GameObject.h"
namespace TinyEngine {
    struct ComponentInfo
    {
        std::string name;

        std::function<void(TinyEngine::GameObject&)> create;

        std::vector<FieldInfo> fields;
    };

 }