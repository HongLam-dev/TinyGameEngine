#include "CameraFollow.h"
#include <iostream>

namespace TinyEngine {
	void CameraFollow::Update()
	{
		if (target != nullptr)
		{
			GetTransform().SetPosition(target->GetPosition());
		}
	}

}