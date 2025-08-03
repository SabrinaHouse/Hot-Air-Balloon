#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"


class TiledRender
{
private:
	float backgrounSize;
	sf::Vector2f center;
public:

	void updateCenterTile(sf::RenderWindow* window, sf::Vector2f& centerTile,
		Camera& camera, float backgroundSize, float backgroundScale);

	void render(sf::RenderWindow* window, sf::Texture backgroundTexture,
		float backgroundSize, sf::Vector2f scale, sf::Vector2f centerTile);

};

