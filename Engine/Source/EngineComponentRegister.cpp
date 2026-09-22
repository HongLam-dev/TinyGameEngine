#include "EngineComponentRegister.h"
#include "SpriteRenderer.h"
#include "Camera.h"
#include "Rigidbody2D.h"
#include "Animator.h"
#include "MakeField.h"

namespace TinyEngine{
    void EngineComponentRegister::RegisterEngineComponents(
        TinyEngine::ComponentRegister& componentRegister)
    {
           componentRegister.RegisterComponent<Transform>(
               {
                   "Transform",
                   [](GameObject& gameObject)
                   {
                       gameObject.AddComponent<Transform>();
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
                   [](GameObject& gameObject)
                   {
                       gameObject.AddComponent<SpriteRenderer>();
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
               [](GameObject& gameObject)
               {
                   gameObject.AddComponent<BoxCollider2D>();
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
                   )
               }
           }
       );

       componentRegister.RegisterComponent<Rigidbody2D>(
           {
               "Rigidbody 2D",
               [](GameObject& gameObject)
               {
                   gameObject.AddComponent<Rigidbody2D>();
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
                   )
               }
           }
       );

       componentRegister.RegisterComponent<Camera>(
           {
               "Camera",
               [](GameObject& gameObject)
               {
                   gameObject.AddComponent<Camera>();
               },
               {}
           }
       );

       componentRegister.RegisterComponent<Animator>(
           {
               "Animation Controller",
               [](GameObject& gameObject)
               {
                   gameObject.AddComponent<Animator>();
               },
               {}
           }
       );

    }
}