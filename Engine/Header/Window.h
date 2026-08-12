#pragma once

#include <SFML/Graphics.hpp>
namespace TinyEngine
{
	class Window
	{
	public:

		Window()
			: sfmlWindow(sf::VideoMode({ 800, 600 }), "Tiny Game Engine")
		{}

		bool IsOpen();
		void Close();
		std::optional<sf::Event> PollEvent();
		void Display();
		void Draw(const sf::Drawable& object);
		void Clear();
	private:
		sf::RenderWindow sfmlWindow;
	};
}