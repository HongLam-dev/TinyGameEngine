#pragma once
#include <SFML/Graphics.hpp>
#include "Vector3.h"
#include "BoxCollider2D.h"
#include "Camera.h"
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
		void DrawCollider(const BoxCollider2D& collider,const Camera& camera);
		void Clear();
		void SetPosition(const Vector3& position);
		sf::Vector2i GetPosition() { return sfmlWindow.getPosition(); }
		sf::Vector2u GetSize() { return sfmlWindow.getSize(); }
	private:
		sf::RenderWindow sfmlWindow;
	};
}