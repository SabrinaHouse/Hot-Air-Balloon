#include "BirdPoints.h"


BirdPoints::BirdPoints(sf::Texture& t, sf::Vector2f p, sf::Vector2f s, float pV)
{ 
	texture = t;
	position = p;
	scale = s;
	pointValue = pV;
}

void BirdPoints::render(sf::RenderWindow* window) {
	sf::Sprite sprite(texture);

	sprite.setPosition(position);
	sprite.setScale(scale);



	window->draw(sprite);

}

