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
						FieldType::Float
					}
				}
			}
		
		);

		componentRegister.RegisterComponent<CameraFollow>(
			{ "Camera Follow",
			[](GameObject& gameObject) {
			gameObject.AddComponent<CameraFollow>();
			},
				{
					{
						"Move Speed",
						FieldType::Float
					}
				}
			}

		);

		componentRegister.RegisterComponent<PingPongAroundCenter>(
			{ "Ping Pong Around Center",
			[](GameObject& gameObject) {
			gameObject.AddComponent<PingPongAroundCenter>();
			},
				{
					{
						"Radius",
						FieldType::Float
					},
					{
						"Direction",
						FieldType::Vector3
					},
					{
						"Speed",
						FieldType::Float
					}
				}
			}

		);
	}
}