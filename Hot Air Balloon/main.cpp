#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Resources.h"
#include <iostream>

int main()
{
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ 1900, 1800 }), "Hot Air Balloon :3");

	sf::Texture balloonTexture;

	if (!balloonTexture.loadFromFile("resources/textures/Balloon.png"))
	{
		std::cout << "Could not load file resources/textures/Balloon.png" << std::endl;
		return -1;
	}

	sf::Sprite balloon(balloonTexture);
	balloon.setPosition({ 150, 150 });
	balloon.setScale({ 20, 20 });
	balloon.setTextureRect(sf::IntRect({ 0,0 }, { 11 ,16 }));

	while (window->isOpen())
	{
		while (const std::optional event = window->pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window->close();
		}

		sf::Vector2f RequestedPlayerMovement(0.0f, 0.0f);
		//gather player input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
		{
			RequestedPlayerMovement += sf::Vector2f(0.0f, -1.5f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
		{
			RequestedPlayerMovement += sf::Vector2f(-1.5f, 0.0f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
		{
			RequestedPlayerMovement += sf::Vector2f(0.0f, 1.5f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
		{
			RequestedPlayerMovement += sf::Vector2f(1.5f, 0.0f);
		}

		balloon.move(RequestedPlayerMovement);

		window->clear();
		window->draw(balloon);
		window->display();

	}

	delete window;
	return 0;

}
