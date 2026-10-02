#include "EngineComponentRegister.h"
#include "SpriteRenderer.h"
#include "Camera.h"
#include "Rigidbody2D.h"
#include "Animator.h"
#include "MakeField.h"
#include "Image.h"
#include "UITransform.h"
#include "EnumStringMap.h"

namespace TinyEngine{
    void EngineComponentRegister::RegisterEngineComponents()
    {
        ComponentRegister& componentRegister = ComponentRegister::Instance();
           componentRegister.RegisterComponent<Transform>(
               {
                   "Transform",
                   [](GameObject& gameObject)->Component&
                   {
                       return gameObject.AddComponent<Transform>();
                   },
                   {
                       MakeField<Transform, Vector3>(
                           "Position",
                           FieldType::Vector3,
                           &Transform::GetPosition,
                           &Transform::SetPosition
                       ),

                       MakeField<Transform, Vector3>(
                           "Rotation",
                           FieldType::Vector3,
                           &Transform::GetRotation,
                           &Transform::SetRotation
                       ),

                       MakeField<Transform, Vector3>(
                           "Scale",
                           FieldType::Vector3,
                           &Transform::GetScale,
                           &Transform::SetScale
                       )
                   }
               }
           );

           componentRegister.RegisterComponent<SpriteRenderer>(
               {
                   "Sprite Renderer",
                   [](GameObject& gameObject)->Component&
                   {
                       return gameObject.AddComponent<SpriteRenderer>();
                   },
                   {
                       MakeField<SpriteRenderer, std::string>(
                           "Texture",
                           FieldType::String,
                           &SpriteRenderer::GetTexturePath,
                           &SpriteRenderer::SetTexture
                       ),

                       MakeField<SpriteRenderer, sf::IntRect>(
                           "Texture Rect",
                           FieldType::IntRect,
                           &SpriteRenderer::GetTextureRect,
                           &SpriteRenderer::SetTextureRect
                       )
                   }
               }
           );

       componentRegister.RegisterComponent<BoxCollider2D>(
           {
               "Box Collider 2D",
               [](GameObject& gameObject)->Component&
               {
                   return gameObject.AddComponent<BoxCollider2D>();
               },
               {
                   MakeField<BoxCollider2D, Vector3>(
                       "Offset",
                       FieldType::Vector3,
                       &BoxCollider2D::GetOffset,
                       &BoxCollider2D::SetOffset
                   ),

                   MakeField<BoxCollider2D, Vector3>(
                       "Size",
                       FieldType::Vector3,
                       &BoxCollider2D::GetSize,
                       &BoxCollider2D::SetSize
                   ),
                        MakeField<BoxCollider2D, bool>(
                       "Is Trigger",
                       FieldType::Bool,
                       &BoxCollider2D::IsTrigger,
                       &BoxCollider2D::SetIsTrigger
                   )
               }
           }
       );

       componentRegister.RegisterComponent<Rigidbody2D>(
           {
               "Rigidbody 2D",
               [](GameObject& gameObject)->Component&
               {
                   return gameObject.AddComponent<Rigidbody2D>();
               },
               {
                   MakeField<Rigidbody2D, float>(
                       "Mass",
                       FieldType::Float,
                       &Rigidbody2D::GetMass,
                       &Rigidbody2D::SetMass
                   ),

                   MakeField<Rigidbody2D, float>(
                       "Gravity Factor",
                       FieldType::Float,
                       &Rigidbody2D::GetGravityScale,
                       &Rigidbody2D::SetGravityScale
                   ),
                    MakeEnumField<Rigidbody2D, CollisionDetectionMode>(
                       "Collision Detect Mode",
                       &Rigidbody2D::GetCollisionDetectMode,
                       &Rigidbody2D::SetCollisionDetectMode
                   ),
                       MakeField<Rigidbody2D, bool>(
                       "Is Kinematic",
                       FieldType::Bool,
                       &Rigidbody2D::IsKinematic,
                       &Rigidbody2D::SetKinematic
                   )
               }
           }
       );

       componentRegister.RegisterComponent<Camera>(
           {
               "Camera",
               [](GameObject& gameObject)->Component&
               {
                   return gameObject.AddComponent<Camera>();
               },
               {}
           }
       );

       componentRegister.RegisterComponent<Animator>(
           {
               "Animation Controller",
               [](GameObject& gameObject)->Component&
               {
                   return gameObject.AddComponent<Animator>();
               },
               {}
           }
       );


       componentRegister.RegisterComponent<Image>(
           {
               "Image",
               [](GameObject& gameObject)->Component&
               {
                   return gameObject.AddComponent<Image>();
               },
               {
                   MakeField<Image, std::string>(
                       "Texture",
                       FieldType::String,
                       &Image::GetTexturePath,
                       &Image::SetTexture
                   ),

                   MakeField<Image, sf::IntRect>(
                       "Texture Rect",
                       FieldType::IntRect,
                       &Image::GetTextureRect,
                       &Image::SetTextureRect
                   )
               }
           }
       );


       componentRegister.RegisterComponent<UITransform>(
           {
               "UI Transform",
               [](GameObject& gameObject)->Component&
               {
                   return gameObject.AddComponent<UITransform>();
               },
               {
                   MakeField<UITransform, Vector3>(
                       "Position",
                       FieldType::Vector3,
                       &UITransform::GetPosition,
                       &UITransform::SetPosition
                   ),

                   MakeField<Transform, Vector3>(
                       "Rotation",
                       FieldType::Vector3,
                       &UITransform::GetRotation,
                       &UITransform::SetRotation
                   ),

                   MakeField<UITransform, Vector3>(
                       "Scale",
                       FieldType::Vector3,
                       &UITransform::GetScale,
                       &UITransform::SetScale
                   )
               }
           }
       );

    }

    void EngineComponentRegister::RegisterEngineEnums() {
        ComponentRegister& componentRegister = ComponentRegister::Instance();

        componentRegister.RegisterEnumClass<CollisionDetectionMode>(
            {
                "Collision Detection Mode",
                {
                    {
                        "Discrete",
                        static_cast<int>(CollisionDetectionMode::Discrete)
                    },
                    {
                        "Continuous",
                        static_cast<int>(CollisionDetectionMode::Continuous)
                    }
                }
            }
            );
    }
}