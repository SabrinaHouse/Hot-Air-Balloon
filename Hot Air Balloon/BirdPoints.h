#pragma once
#include <SFML/Graphics.hpp>

// collectable birds spawn randomly around the player, despawning when too far away

class BirdPoints
{
public:
	float pointValue;
	sf::Vector2f position; 
	sf::Vector2f scale;
	void move(float deltaTime);
	void setAngle();

	BirdPoints(sf::Texture& t, sf::Vector2f p, sf::Vector2f s, float pV, float speed);
	bool checkPlayerCollision(sf::Sprite player);
	void render(sf::RenderWindow* window);
private:
	sf::Texture texture;
	sf::Sprite* sprite;
	sf::Clock movementClock;
	float angle = (std::rand() %360);
	float speed;

};