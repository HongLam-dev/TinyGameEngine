#include "Window.h"
namespace TinyEngine
{

	bool TinyEngine::Window::IsOpen()
	{
		return sfmlWindow.isOpen();
	}

	void TinyEngine::Window::Close()
	{
		sfmlWindow.close();
	}

	std::optional<sf::Event> TinyEngine::Window::PollEvent() {
		return sfmlWindow.pollEvent();
	}
	void TinyEngine::Window::Display()
	{
		sfmlWindow.display();
	}

	void TinyEngine::Window::Clear()
	{
		sfmlWindow.clear();
	}

	void TinyEngine::Window::Draw(const sf::Drawable& object)
	{
		sfmlWindow.draw(object);
	}

}