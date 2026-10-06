#pragma once
#include "FieldInfo.h"

namespace TinyEngine {
    template<typename ComponentType, typename ValueType>
    FieldInfo MakeField(
        const std::string& name,
        FieldType type,
        ValueType(ComponentType::* getter)() const,
        void (ComponentType::* setter)(ValueType)
    )
    {
        FieldInfo field;

        field.name = name;
        field.type = type;
        field.valueTypeID = typeid(ValueType);

        field.getValue =
            [getter](const Component& component) -> std::any
            {
                const ComponentType& typedComponent =
                    static_cast<const ComponentType&>(component);

                ValueType value = (typedComponent.*getter)();

                if constexpr (std::is_enum_v<ValueType>)
                {
                    return static_cast<int>(value);
                }
                else
                {
                    return value;
                }
            };

        field.setValue =
            [setter](Component& component, const std::any& value) -> bool
            {
                ComponentType& typedComponent =
                    static_cast<ComponentType&>(component);

                if constexpr (std::is_enum_v<ValueType>)
                {
                    if (value.type() != typeid(int))
                        return false;

                    int intValue =
                        std::any_cast<int>(value);

                    (typedComponent.*setter)(
                        static_cast<ValueType>(intValue)
                        );
                }
                else
                {
                    if constexpr (
                        std::is_pointer_v<ValueType> &&
                        std::derived_from<
                        std::remove_pointer_t<ValueType>,
                        Component>)
                    {
                        if (value.type() == typeid(Component*))
                        {
                            Component* component =
                                std::any_cast<Component*>(value);

                            using RequiredComponent =
                                std::remove_pointer_t<ValueType>;

                            RequiredComponent* requiredComponent =
                                dynamic_cast<RequiredComponent*>(component);

                            if (!requiredComponent)
                                return false;

                            (typedComponent.*setter)(requiredComponent);
                            return true;
                        }
                    }
                    else {
                        if (value.type() != typeid(ValueType))
                            return false;

                        (typedComponent.*setter)(
                            std::any_cast<ValueType>(value)
                            );
                    }
                }

                return true;
            };

        if constexpr (
            std::is_pointer_v<ValueType> &&
            std::derived_from<
            std::remove_pointer_t<ValueType>,
            Component>
            )
        {
            field.getReferencedComponent =
                [](const std::any& value) -> Component*
                {
                    ValueType referenced =
                        std::any_cast<ValueType>(value);

                    return referenced;
                };
        }

        return field;
    }
}