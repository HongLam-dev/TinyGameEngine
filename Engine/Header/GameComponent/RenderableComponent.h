#pragma once
#include "Component.h"

namespace TinyEngine {
	class RenderableComponent:public Component
	{
	public:
		RenderableComponent(GameObject& owner);
		void OnDestroy() override;
	private:

	};

}