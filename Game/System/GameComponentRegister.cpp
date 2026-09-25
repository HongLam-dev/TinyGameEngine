#include "GameComponentRegister.h"
#include "ComponentRegister.h"
#include "ComponentInfo.h"
#include "PlayerController.h"
#include "CameraFollow.h"
#include "PingPong.h"
#include "MakeField.h"
#include "Vector3.h"
using namespace TinyEngine;

namespace TGModule{
	void GameComponentRegister::RegisterGameComponents() {
        ComponentRegister& componentRegister = ComponentRegister::Instance();

        componentRegister.RegisterComponent<TinyGame::PlayerController>(
            {
                "Player Controller",
                [](GameObject& gameObject)->Component&
                {
                    return gameObject.AddComponent<TinyGame::PlayerController>();
                },
                {
                    MakeField<TinyGame::PlayerController, float>(
                        "Move Speed",
                        FieldType::Float,
                        &TinyGame::PlayerController::GetMoveSpeed,
                        &TinyGame::PlayerController::SetMoveSpeed
                    )
                }
            }
        );

        componentRegister.RegisterComponent<TinyGame::PingPongAroundCenter>(
            {
                "Ping Pong Around Center",
                [](GameObject& gameObject)->Component&
                {
                    return gameObject.AddComponent<TinyGame::PingPongAroundCenter>();
                },
                {
                    MakeField<TinyGame::PingPongAroundCenter, float>(
                        "Radius",
                        FieldType::Float,
                        &TinyGame::PingPongAroundCenter::GetRadius,
                        &TinyGame::PingPongAroundCenter::SetRadius
                    ),

                    MakeField<TinyGame::PingPongAroundCenter, Vector3>(
                        "Direction",
                        FieldType::Vector3,
                        &TinyGame::PingPongAroundCenter::GetDirection,
                        &TinyGame::PingPongAroundCenter::SetDirection
                    ),

                    MakeField<TinyGame::PingPongAroundCenter, float>(
                        "Speed",
                        FieldType::Float,
                        &TinyGame::PingPongAroundCenter::GetSpeed,
                        &TinyGame::PingPongAroundCenter::SetSpeed
                    )
                }
            }
        );

        componentRegister.RegisterComponent<TinyGame::CameraFollow>(
            {
                "Camera Follow",
                [](GameObject& gameObject)->Component&
                {
                    return gameObject.AddComponent<TinyGame::CameraFollow>();
                },
                {
                }
            }
        );

	}
}