#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "TinyGameEngine.h"

using namespace std;

int main()
{
	sf::RenderWindow window(sf::VideoMode({800,600}),"SFML window");

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			// Close window: exit
			if (event->is<sf::Event::Closed>())
				window.close();
		}
		window.display();

	}
	return 0;
}
