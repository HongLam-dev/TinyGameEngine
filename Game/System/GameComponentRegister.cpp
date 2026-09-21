#include "GameComponentRegister.h"
#include "ComponentRegister.h"
#include "ComponentInfo.h"
#include "PlayerController.h"
#include "CameraFollow.h"
#include "PingPong.h"
using namespace TinyEngine;

namespace TinyGame{
	void GameComponentRegister::RegisterGameComponents(TinyEngine::ComponentRegister& componentRegister) {
		componentRegister.RegisterComponent<PlayerController>(
			{"Player Controller",
			[](GameObject& gameObject){
			gameObject.AddComponent<PlayerController>();
			},
				{
					{
						"Move Speed",
						FieldType::Float,

						[](const Component& component) -> std::any
						{
							return static_cast<const PlayerController&>(component).GetMoveSpeed();
						},

						[](Component& component, const std::any& value)
						{
							static_cast<PlayerController&>(component)
								.SetMoveSpeed(std::any_cast<float>(value));
						}
					}
				}
			}
		
		);

        componentRegister.RegisterComponent<PingPongAroundCenter>(
            {
                "Ping Pong Around Center",
                [](GameObject& gameObject)
                {
                    gameObject.AddComponent<PingPongAroundCenter>();
                },
                {
                    {
                        "Radius",
                        FieldType::Float,

                        [](const Component& component) -> std::any
                        {
                            return static_cast<const PingPongAroundCenter&>(
                                component
                            ).GetRadius();
                        },

                        [](Component& component, const std::any& value)
                        {
                            static_cast<PingPongAroundCenter&>(
                                component
                            ).SetRadius(
                                std::any_cast<float>(value)
                            );
                        }
                    },
                    {
                        "Direction",
                        FieldType::Vector3,

                        [](const Component& component) -> std::any
                        {
                            return static_cast<const PingPongAroundCenter&>(
                                component
                            ).GetDirection();
                        },

                        [](Component& component, const std::any& value)
                        {
                            static_cast<PingPongAroundCenter&>(
                                component
                            ).SetDirection(
                                std::any_cast<Vector3>(value)
                            );
                        }
                    },
                    {
                        "Speed",
                        FieldType::Float,

                        [](const Component& component) -> std::any
                        {
                            return static_cast<const PingPongAroundCenter&>(
                                component
                            ).GetSpeed();
                        },

                        [](Component& component, const std::any& value)
                        {
                            static_cast<PingPongAroundCenter&>(
                                component
                            ).SetSpeed(
                                std::any_cast<float>(value)
                            );
                        }
                    }
                }
            }
        );

        componentRegister.RegisterComponent<CameraFollow>(
            {
                "Camera Follow",
                [](GameObject& gameObject)
                {
                    gameObject.AddComponent<CameraFollow>();
                },
                {
                   
                }
            }
        );
	}
}