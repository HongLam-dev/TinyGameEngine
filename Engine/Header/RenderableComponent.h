#pragma once
#include "Component.h"

namespace TinyEngine {
	class RenderableComponent:public Component
	{
	public:
		RenderableComponent(GameObject& owner) :Component(owner) {}
		void Start() override;
		void OnDestroy() override;
	private:

	};

}