#pragma once
#include <SFML/Graphics.hpp>

namespace TinyEngine
{
	class Input
	{
	public:
		bool isKeyPressed(sf::Keyboard::Key key);
		bool OnKeyDown(sf::Keyboard::Key key);
		bool OnKeyUp(sf::Keyboard::Key key);
		void ProcessEvent(const sf::Event& event);
		void SyncKeyState() { previousKeyStates = keyStates; }
		static Input& Get()
		{
			static Input instance;
			return instance;
		}

	private:
		std::array<bool, static_cast<std::size_t>(sf::Keyboard::KeyCount)> previousKeyStates{};
		std::array<bool, static_cast<std::size_t>(sf::Keyboard::KeyCount)> keyStates{};
	};
}