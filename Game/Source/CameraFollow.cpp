#include "CameraFollow.h"
#include <iostream>
using namespace TinyEngine;

namespace TinyGame {
	void CameraFollow::Update()
	{
		if (target != nullptr)
		{
			GetTransform().SetPosition(target->GetPosition());
		}
	}

}