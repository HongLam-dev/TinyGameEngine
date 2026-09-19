#include "Camera.h"
#include "EngineSettings.h"
#include "Vector3.h"
#include "Vector2.h"

namespace TinyEngine {

	void Camera::Update(float deltaTime) {
	}
	Vector2 Camera::WorldToScreenPosition(Vector3 worldPos, Vector2 windowSize) const {
		Vector3 camPosition = worldPos - GetTransform().GetPosition();
		Vector2 relativePixels{
		WorldToPixels(camPosition.x),
		WorldToPixels(camPosition.y)
		};

		Vector2 windowCenter{
			windowSize.x / 2.f,
			windowSize.y / 2.f
		};

		Vector2 objectScreenPosition = windowCenter + relativePixels;
		return objectScreenPosition;
	}
	Vector3 Camera::ScreenToWorldPosition(
		Vector2 screenPos,
		Vector2 windowSize) const
	{
		Vector2 windowCenter{
			windowSize.x / 2.f,
			windowSize.y / 2.f
		};

		Vector2 relativePixels =
			screenPos - windowCenter;

		Vector3 camPosition{
			PixelsToWorld(relativePixels.x),
			-PixelsToWorld(relativePixels.y),
			0
		};

		return camPosition + GetTransform().GetPosition();
	}
}