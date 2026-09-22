#include "GameComponentRegister.h"
#include "ComponentRegister.h"
#include "ComponentInfo.h"
#include "PlayerController.h"
#include "CameraFollow.h"
#include "PingPong.h"
#include "MakeField.h"
#include "Vector3.h"
using namespace TinyEngine;

namespace TinyGame{
	void GameComponentRegister::RegisterGameComponents(TinyEngine::ComponentRegister& componentRegister) {
        componentRegister.RegisterComponent<PlayerController>(
            {
                "Player Controller",
                [](GameObject& gameObject)
                {
                    gameObject.AddComponent<PlayerController>();
                },
                {
                    MakeField<PlayerController, float>(
                        "Move Speed",
                        FieldType::Float,
                        &PlayerController::GetMoveSpeed,
                        &PlayerController::SetMoveSpeed
                    )
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
                    MakeField<PingPongAroundCenter, float>(
                        "Radius",
                        FieldType::Float,
                        &PingPongAroundCenter::GetRadius,
                        &PingPongAroundCenter::SetRadius
                    ),

                    MakeField<PingPongAroundCenter, Vector3>(
                        "Direction",
                        FieldType::Vector3,
                        &PingPongAroundCenter::GetDirection,
                        &PingPongAroundCenter::SetDirection
                    ),

                    MakeField<PingPongAroundCenter, float>(
                        "Speed",
                        FieldType::Float,
                        &PingPongAroundCenter::GetSpeed,
                        &PingPongAroundCenter::SetSpeed
                    )
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