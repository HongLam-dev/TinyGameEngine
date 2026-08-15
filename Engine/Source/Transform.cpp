#include "Transform.h"
#include <iostream>

namespace TinyEngine {
	void Transform::SetPosition(const Vector3& pos)
	{
		this->position = pos;
	}
	void Transform::SetScale(const Vector3& newScale) {
		this->scale = newScale;
	}
	void Transform::SetRotation(const Vector3& newRotation)
	{
		this->rotation = rotation;
	}
}