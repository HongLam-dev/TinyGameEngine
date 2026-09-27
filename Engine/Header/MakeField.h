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
       return {
           name,
           type,
            typeid(void),
           [getter](const Component& component) -> std::any
           {
               const ComponentType& typedComponent =
                   static_cast<const ComponentType&>(component);

               return (typedComponent.*getter)();
           },

           [setter](Component& component, const std::any& value)
           {
               ComponentType& typedComponent =
                   static_cast<ComponentType&>(component);

               (typedComponent.*setter)(
                   std::any_cast<ValueType>(value)
               );
           }
       };
   }
   template<typename ComponentType, typename EnumType>
   FieldInfo MakeEnumField(
       const std::string& name,
       EnumType(ComponentType::* getter)() const,
       void (ComponentType::* setter)(EnumType)
   )
   {
       FieldInfo field;

       field.name = name;
       field.type = FieldType::Enum;
       field.enumType = typeid(EnumType);

       field.getValue =
           [getter](const Component& component) -> std::any
           {
               const ComponentType& typed =
                   static_cast<const ComponentType&>(component);

               return static_cast<int>(
                   (typed.*getter)()
                   );
           };

       field.setValue =
           [setter](Component& component, const std::any& value)
           {
               ComponentType& typed =
                   static_cast<ComponentType&>(component);

               int intValue = std::any_cast<int>(value);

               (typed.*setter)(
                   static_cast<EnumType>(intValue)
                   );
           };

       return field;
   }
}