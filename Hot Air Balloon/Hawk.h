#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

//hawks spawn at the edges of the screen and shoot off in a straight line towards the player

class Hawk 
{

public:
	sf::Vector2f position;

	Hawk(sf::Texture texture, sf::Vector2f position, sf::Vector2f scale, float speed, sf::Sprite player);
	void render(sf::RenderWindow* window);
	void move(float deltaTime);
	bool checkCollision(sf::Sprite player);
	bool despawnHawk(sf::Sprite player,int despawnRadius);
	

private:
	float angle;
	float speed;
	sf::Sprite* sprite;
	sf::Texture texture;
};

