#pragma once
#include "Component.h"
#include "Image.h"
#include <functional>

namespace TinyEngine {
	class Button : public Component
	{
	public:
		void AddCallback(std::function<void()> callback) { callbacks.push_back(callback); };
		void Invoke();

		Image* GetTargetArea() { return targetGraphic; }
		void SetTargetArea(Image* image) { this->targetGraphic = targetGraphic; };

		void Start() override;
		void Update(float deltaTime) override;
	private:
		Image* targetGraphic=nullptr;
		std::vector<std::function<void()>> callbacks;
	};

}