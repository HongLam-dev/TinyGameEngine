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
		else if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
		{
			mouseButtonStates[static_cast<std::size_t>(mousePressed->button)] = true;
		}
		else if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>())
		{
			mouseButtonStates[static_cast<std::size_t>(mouseReleased->button)] = false;
		}
		else if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>())
		{
			mounseDelta = mousePosition - mouseMoved->position;
			mousePosition = mouseMoved->position;
		}
	}

	bool Input::OnKeyDown(sf::Keyboard::Key key) {
		size_t code = static_cast<std::size_t>(key);
		if (!previousKeyStates[code] && keyStates[code])
			return true;
		return false;
	}
	bool Input::OnKeyUp(sf::Keyboard::Key key) {
		size_t code = static_cast<std::size_t>(key);
		if (previousKeyStates[code] && !keyStates[code])
			return true;
		return false;
	}

	bool Input::OnMouseDown(sf::Mouse::Button button) {
		size_t code = static_cast<std::size_t>(button);
		if (!previousMouseButtonStates[code] && mouseButtonStates[code])
			return true;
		return false;
	}
	bool Input::OnMouseUp(sf::Mouse::Button button) {
		size_t code = static_cast<std::size_t>(button);
		if (previousMouseButtonStates[code] && !mouseButtonStates[code])
			return true;
		return false;
	}
	bool Input::IsMousePressed(sf::Mouse::Button button) {
		return mouseButtonStates[static_cast<size_t>(button)];
	}

	bool Input::isKeyPressed(sf::Keyboard::Key key) {

		return keyStates[static_cast<size_t>(key)];
	}

	void Input::EndFrame()
	{ 
		previousKeyStates = keyStates;
		previousMouseButtonStates = mouseButtonStates;
		mounseDelta.x = 0;
		mounseDelta.y = 0;
	}

}