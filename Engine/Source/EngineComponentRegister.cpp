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
                        FieldType::Vector3,

                        [](const Component& component) -> std::any
                        {
                            return static_cast<const Transform&>(component).GetPosition();
                        },

                        [](Component& component, const std::any& value)
                        {
                            static_cast<Transform&>(component)
                                .SetPosition(std::any_cast<Vector3>(value));
                        }
                    },
                    {
                        "Rotation",
                        FieldType::Vector3,

                        [](const Component& component) -> std::any
                        {
                            return static_cast<const Transform&>(component).GetRotation();
                        },

                        [](Component& component, const std::any& value)
                        {
                            static_cast<Transform&>(component)
                                .SetRotation(std::any_cast<Vector3>(value));
                        }
                    },
                    {
                        "Scale",
                        FieldType::Vector3,

                        [](const Component& component) -> std::any
                        {
                            return static_cast<const Transform&>(component).GetScale();
                        },

                        [](Component& component, const std::any& value)
                        {
                            static_cast<Transform&>(component)
                                .SetScale(std::any_cast<Vector3>(value));
                        }
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
                        FieldType::Vector3,

                        [](const Component& component) -> std::any
                        {
                            return static_cast<const BoxCollider2D&>(component).GetOffset();
                        },

                        [](Component& component, const std::any& value)
                        {
                            static_cast<BoxCollider2D&>(component)
                                .SetOffset(std::any_cast<Vector3>(value));
                        }
                    },
                    {
                        "Size",
                        FieldType::Vector3,

                        [](const Component& component) -> std::any
                        {
                            return static_cast<const BoxCollider2D&>(component).GetSize();
                        },

                        [](Component& component, const std::any& value)
                        {
                            static_cast<BoxCollider2D&>(component)
                                .SetSize(std::any_cast<Vector3>(value));
                        }
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
                        FieldType::Float,

                        [](const Component& component) -> std::any
                        {
                            return static_cast<const Rigidbody2D&>(component).GetMass();
                        },

                        [](Component& component, const std::any& value)
                        {
                            static_cast<Rigidbody2D&>(component)
                                .SetMass(std::any_cast<float>(value));
                        }
                    },
                    {
                        "Gravity Factor",
                        FieldType::Float,

                        [](const Component& component) -> std::any
                        {
                            return static_cast<const Rigidbody2D&>(component).GetGravityScale();
                        },

                        [](Component& component, const std::any& value)
                        {
                            static_cast<Rigidbody2D&>(component)
                                .SetGravityScale(std::any_cast<float>(value));
                        }
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
                {}
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