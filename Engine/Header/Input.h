#pragma once
#include <SFML/Graphics.hpp>

namespace TinyEngine
{
	class Input
	{
	public:
		bool isKeyPressed(sf::Keyboard::Key key);
		void ProcessEvent(const sf::Event& event);
	private:
		std::array<bool, static_cast<std::size_t>(sf::Keyboard::KeyCount)> keyStates{};

	};
}