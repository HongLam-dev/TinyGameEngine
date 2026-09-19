#include "EngineComponentRegister.h"
#include "SpriteRenderer.h"
#include "Camera.h"
#include "Rigidbody2D.h"
#include "Animator.h"

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
                    {
                        "Position",
                        FieldType::Vector3
                    },
                    {
                        "Rotation",
                        FieldType::Vector3
                    },
                    {
                        "Scale",
                        FieldType::Vector3
                    }
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
                    // Fields will go here later
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
                    {
                        "Offset",
                        FieldType::Vector2
                    },
                    {
                        "Size",
                        FieldType::Vector2
                    }
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
                    {
                        "Mass",
                        FieldType::Float
                    },
                    {
                        "Gravity Factor",
                        FieldType::Float
                    }
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
                {
                }
            }
        );

        componentRegister.RegisterComponent<Animator>(
            {
                "Animation Controller",
                [](GameObject& gameObject)
                {
                    gameObject.AddComponent<Animator>();
                },
                {
                }
            }
        );
    }
}