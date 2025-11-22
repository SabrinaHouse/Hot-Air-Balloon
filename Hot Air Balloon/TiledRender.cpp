#include "TiledRender.h"

//updates a 3x3 square of the background around the player as they move so that it appears infinite

//checks where the player currently is and updates where the center tile needs to be based on that
void TiledRender::updateCenterTile(sf::RenderWindow* window, sf::Vector2f& centerTile,
	Camera& camera, float backgroundSize, float backgroundScale) {

	sf::Vector2f cameraCenter = camera.GetView(window->getSize()).getCenter();
	if (cameraCenter.x > centerTile.x + backgroundSize * backgroundScale / 2) // check right
	{
		centerTile.x += backgroundSize * backgroundScale;
	} else if (cameraCenter.x < centerTile.x - backgroundSize * backgroundScale / 2) // check left
	{
		centerTile.x -= backgroundSize * backgroundScale;
	}

	if (cameraCenter.y > centerTile.y + backgroundSize * backgroundScale / 2) // check bttom
	{
		centerTile.y += backgroundSize * backgroundScale;
	}
	else if (cameraCenter.y < centerTile.y - backgroundSize * backgroundScale / 2) // check top
	{
		centerTile.y -= backgroundSize * backgroundScale;
	}
}

//renders the tiles around the center tile
void TiledRender::render(sf::RenderWindow* window, sf::Texture backgroundTexture,
	float backgroundSize, sf::Vector2f scale, sf::Vector2f centerTile) {

	sf::Sprite background(backgroundTexture);
	background.setOrigin({ backgroundTexture.getSize().x / 2.f, backgroundTexture.getSize().y / 2.f });
	background.setScale(scale);

	for (int y = -1; y <= 1; y++) 
	{
		for (int x = -1; x <= 1; x++)
		{
			float posX = x * backgroundSize * scale.x;
			float posY = y * backgroundSize * scale.y;

			background.setPosition({ centerTile.x + posX, centerTile.y + posY });

			window->draw(background);
		}
	}
}

