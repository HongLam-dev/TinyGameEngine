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

}