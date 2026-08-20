#pragma once
#include "Component.h"

namespace TinyEngine {
    class IComponentObserver
    {
    public:
        virtual ~IComponentObserver() = default;

        virtual void OnComponentAdded(Component& component) = 0;
        virtual void OnComponentRemoved(Component& component) = 0;
    };
}