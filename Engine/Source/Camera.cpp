#include "Camera.h"
#include "EngineSettings.h"
#include "Vector3.h"
#include "Vector2.h"

namespace TinyEngine {

	void Camera::Update(float deltaTime) {
	}
	const Vector2& Camera::WorldToScreenPosition(Vector3 worldPos, Vector2 windowSize) const {
		Vector3 camPosition = worldPos - GetTransform().GetPosition();
		Vector2 relativePixels{
		WorldToPixels(camPosition.x),
		WorldToPixels(camPosition.y)
		};

		Vector2 windowCenter{
			windowSize.x / 2.f,
			windowSize.y / 2.f
		};

		Vector2 objectScreenPosition{
			windowCenter.x + relativePixels.x,
			windowCenter.y + relativePixels.y
		};
		return objectScreenPosition;
	}
}