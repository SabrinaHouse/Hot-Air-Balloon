#include "BirdPoints.h"


BirdPoints::BirdPoints(sf::Texture& t, sf::Vector2f p, sf::Vector2f s, float pV, float sp)
{ 
	texture = t;
	position = p;
	scale = s;
	pointValue = pV;
	speed = sp;
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

void BirdPoints::setAngle() {
	angle = (std::rand() % 360);
}

void BirdPoints::move(float deltaTime) {
	if (movementClock.getElapsedTime().asSeconds() > std::rand() % 2 + 2) {
		setAngle();
		movementClock.restart();
	}
	position.x += std::cos(angle * (3.1415 / 180.0)) * speed * deltaTime;
	position.y += std::sin(angle * (3.1415 / 180.0)) * speed * deltaTime;
	sprite->setPosition(position);
}
