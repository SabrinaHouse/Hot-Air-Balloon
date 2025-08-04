#pragma once
#include <SFML/Graphics.hpp>


class BirdPoints
{

public:
	float pointValue;
	sf::Vector2f position; 
	sf::Vector2f scale;

	BirdPoints(sf::Texture& t, sf::Vector2f p, sf::Vector2f s, float pV);
	bool checkPlayerCollision(sf::Sprite player);

	void render(sf::RenderWindow* window);
private:
	sf::Texture texture;
	sf::Sprite* sprite;
	
};

