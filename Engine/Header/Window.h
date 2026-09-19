#pragma once
#include <SFML/Graphics.hpp>
#include "Vector3.h"
#include "Vector2.h"
#include "BoxCollider2D.h"
#include "Camera.h"
namespace TinyEngine
{
	class Window
	{
	public:

		Window()
			: sfmlWindow(sf::VideoMode({ 1280, 720 }), "Tiny Game Engine")
		{}
		sf::RenderWindow* GetRenderWindow() { return &sfmlWindow; }
		bool IsOpen();
		void Close();
		std::optional<sf::Event> PollEvent();
		void Display();
		void Draw(const sf::Drawable& object);
		void DrawCollider(const BoxCollider2D& collider,const Camera& camera);
		void Clear();
		void SetPosition(const Vector3& position);
		Vector2 GetPosition();
		Vector2 GetSize();
	private:
		sf::RenderWindow sfmlWindow;
	};
}