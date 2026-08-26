#pragma once
#include <SFML/Graphics.hpp>
#include "Vector3.h"
#include "BoxCollider2D.h"
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
		void DrawCollider(const BoxCollider2D& collider);
		void Clear();
		void SetPosition(const Vector3& position);
	private:
		sf::RenderWindow sfmlWindow;
	};
}