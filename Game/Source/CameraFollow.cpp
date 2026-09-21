#include "CameraFollow.h"
#include <iostream>
using namespace TinyEngine;

namespace TinyGame {
	void CameraFollow::Start() {
	//	DontDestroyOnload(*this);
	}
	void CameraFollow::Update(float deltaTime)
	{
		if (target != nullptr)
		{
			GetTransform().SetPosition(target->GetPosition());
		}
	}
	
}