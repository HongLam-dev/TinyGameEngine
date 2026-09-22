#include "Transform.h"
#include <iostream>

namespace TinyEngine {
	void Transform::SetPosition(Vector3 pos)
	{
		this->position = pos;
	}
	void Transform::SetScale(Vector3 newScale) {
		this->scale = newScale;
	}
	void Transform::SetRotation(Vector3 newRotation)
	{
		this->rotation = rotation;
	}
}