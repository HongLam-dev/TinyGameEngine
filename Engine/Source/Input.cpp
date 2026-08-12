#include "Input.h"
#include <SFML/Graphics.hpp>
namespace TinyEngine
{

	void Input::ProcessEvent(const sf::Event& event)
	{
		if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
		{
			keyStates[static_cast<std::size_t>(keyPressed->code)] = true;
		}
		else if(const auto* keyPressed = event.getIf<sf::Event::KeyReleased>())
		{
			keyStates[static_cast<std::size_t>(keyPressed->code)] = false;
		}
		else if (event.is<sf::Event::MouseButtonPressed>())
		{
			// mouse event
		}
	}

	bool Input::isKeyPressed(sf::Keyboard::Key key) {

		return keyStates[static_cast<size_t>(key)];
	}

}