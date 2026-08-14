#include "Transform.h"
#include <iostream>

namespace TinyEngine {
	void Transform::SetPosition(const Vector3& pos)
	{
		this->position = pos;
	}
}