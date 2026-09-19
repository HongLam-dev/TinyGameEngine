#pragma once
#include "ComponentRegister.h"

namespace TinyGame {
	class  GameComponentRegister
	{
	public:
		//GameComponentRegister(TinyEngine::ComponentRegister& componentRegister);
		static void RegisterGameComponents(TinyEngine::ComponentRegister& componentRegister);
	private:
		//TinyEngine::ComponentRegister& componentRegister;
	};
}