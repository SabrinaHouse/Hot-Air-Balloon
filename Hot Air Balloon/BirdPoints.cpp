#include "BirdPoints.h"


BirdPoints::BirdPoints(sf::Texture& t, sf::Vector2f p, sf::Vector2f s, float pV)
{ 
	texture = t;
	position = p;
	scale = s;
	pointValue = pV;
	sprite = new sf::Sprite(texture);

	sprite->setPosition(position);
	sprite->setScale(scale);
}

void BirdPoints::render(sf::RenderWindow* window) {
	window->draw(*sprite);

}

bool BirdPoints::checkPlayerCollision(sf::Sprite player) {
	if (const std::optional intersection = player.getGlobalBounds().findIntersection(sprite->getGlobalBounds()))
	{
		return true;
	}
	return false;
}
