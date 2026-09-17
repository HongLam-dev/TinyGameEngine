#include "Camera.h"
#include "EngineSettings.h"
#include "Vector3.h"

namespace TinyEngine {

	void Camera::Update(float deltaTime) {
	}
	const Vector3& Camera::WorldToScreenPosition(Vector3 worldPos, Vector2 windowSize) const {
		Vector3 camPosition = worldPos - GetTransform().GetPosition();
		Vector3 relativePixels{
		WorldToPixels(camPosition.x),
		WorldToPixels(camPosition.y),
		0.f
		};

		Vector3 windowCenter{
			windowSize.x / 2.f,
			windowSize.y / 2.f,
			0.f
		};

		Vector3 objectScreenPosition{
			windowCenter.x + relativePixels.x,
			windowCenter.y + relativePixels.y,
			0.f
		};
		return objectScreenPosition;
	}
}