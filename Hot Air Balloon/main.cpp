#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"
#include "Resources.h"
#include "Camera.h"

Camera camera(1080);

int main()
{

	sf::Clock frameClock;

	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ 1900, 1800 }), "Hot Air Balloon :3");

	//sf::Texture balloonTexture;

	//if (!balloonTexture.loadFromFile("resources/textures/Balloon.png"))
	//{
	//	std::cout << "Could not load file resources/textures/Balloon.png" << std::endl;
	//	return -1;
	//}

	for (const auto& file : std::filesystem::directory_iterator("./resources/textures"))
	{
		if (file.is_regular_file() && (file.path().extension() == ".png")) {
			if (!Resources::textures[file.path().filename().string()].loadFromFile(file.path().string()))
				std::abort();
		}
	}
	sf::Sprite balloon(Resources::textures["Balloon.png"]);
	balloon.setOrigin({ 6, 8 });
	balloon.setPosition({ 0, 0 });
	balloon.setScale({ 10, 10 });

	sf::Sprite background(Resources::textures["Clouds.png"]);
	background.setOrigin({ 288, 162 });
	background.setPosition({ 0, 0 });
	background.setScale({ 10, 10 });
	//balloon.setTextureRect(sf::IntRect({ 0,0 }, { 11 ,16 }));

	camera.position = { 0, 0 };

	while (window->isOpen())
	{
		float deltaTime = frameClock.getElapsedTime().asSeconds();
		frameClock.restart();

		while (const std::optional event = window->pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window->close();
		}

		float playerSpeed = 400;

		sf::Vector2f velocity(0.0f, 0.0f);
		//gather player input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
		{
			velocity += sf::Vector2f(0.0f, -playerSpeed * deltaTime);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
		{
			velocity += sf::Vector2f(-playerSpeed * deltaTime, 0.0f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
		{
			velocity += sf::Vector2f(0.0f, playerSpeed * deltaTime);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
		{
			velocity += sf::Vector2f(playerSpeed * deltaTime, 0.0f);
		}

		
		balloon.move(velocity);

		//Camera Movement
		if (camera.position.x - balloon.getPosition().x > 200) //left
		{
			camera.position.x += velocity.x;
		}
		else if (camera.position.x - balloon.getPosition().x < -200) //right
		{
			camera.position.x += velocity.x;
		}

		if (camera.position.y - balloon.getPosition().y > 200) //down
		{
			camera.position.y += velocity.y;
		}
		else if (camera.position.y - balloon.getPosition().y < -200) //up
		{
			camera.position.y += velocity.y;
		}

		window->clear();
		window->setView(camera.GetView(window->getSize()));
		window->draw(background);
		window->draw(balloon);
		window->display();

	}

	delete window;
	return 0;

}
