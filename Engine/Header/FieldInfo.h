#pragma once
#include <string>
#include "FieldType.h"

namespace TinyEngine {
    struct FieldInfo
    {
        std::string name;
        FieldType type;
    };
}