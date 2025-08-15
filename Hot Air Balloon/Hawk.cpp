#include "Hawk.h"

Hawk::Hawk(sf::Texture t, sf::Vector2f position, sf::Vector2f scale, float speed, sf::Sprite player) 
	: position(position), speed(speed)
{
	texture = t;
	sprite = new sf::Sprite(texture);
	sprite->setPosition(position);
	sprite->setScale(scale);
	sprite->setOrigin({ float(texture.getSize().x) / 2, float(texture.getSize().y) / 2 });
	angle = std::atan2(player.getPosition().y - position.y, player.getPosition().x - position.x);
}

void Hawk::render(sf::RenderWindow* window) {
	window->draw(*sprite);
}

void Hawk::move(float deltaTime) {

	position.x += std::cos(angle) * speed * deltaTime;
	position.y += std::sin(angle) * speed * deltaTime;
	sprite->setPosition(position);

}

bool Hawk::checkCollision(sf::Sprite player) {
	if (const std::optional intersection = player.getGlobalBounds().findIntersection(sprite->getGlobalBounds()))
	{
		std::cout << "game over" << std::endl;
		return true;
	}
	return false;
}

bool Hawk::despawnHawk(sf::Sprite player, int despawnRadius) {
	if (player.getPosition().x - position.x > despawnRadius
		|| player.getPosition().y - position.y > despawnRadius
		|| position.x - player.getPosition().x > despawnRadius
		|| position.y - player.getPosition().y > despawnRadius) {
		return true;
	}
	return false;
}
