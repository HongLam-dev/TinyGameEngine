#pragma once
#include <SFML/Graphics.hpp>

namespace TinyEngine
{
	class Input
	{
	public:
		Input() {
			mousePosition = sf::Mouse::getPosition();
		}
		bool isKeyPressed(sf::Keyboard::Key key);
		bool OnKeyDown(sf::Keyboard::Key key);
		bool OnKeyUp(sf::Keyboard::Key key);
		bool OnMouseDown(sf::Mouse::Button button);
		bool OnMouseUp(sf::Mouse::Button button);
		bool IsMousePressed(sf::Mouse::Button button);
		void ProcessEvent(const sf::Event& event);
		sf::Vector2i GetMouseMovement() { return mounseDelta; }

		void EndFrame();
		static Input& Get()
		{
			static Input instance;
			return instance;
		}

	private:
		std::array<bool, static_cast<std::size_t>(sf::Keyboard::KeyCount)> previousKeyStates{};
		std::array<bool, static_cast<std::size_t>(sf::Keyboard::KeyCount)> keyStates{};
		std::array<bool, static_cast<std::size_t>(sf::Mouse::ButtonCount)> mouseButtonStates{};
		std::array<bool, static_cast<std::size_t>(sf::Mouse::ButtonCount)> previousMouseButtonStates{};
		sf::Vector2i mounseDelta;
		sf::Vector2i mousePosition;
	};
}