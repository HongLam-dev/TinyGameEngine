#pragma once
#include "Vector3.h"
namespace TinyEngine{
    struct Bounds
    {
        Vector3 center;
        Vector3 min;
        Vector3 max;
    };
}