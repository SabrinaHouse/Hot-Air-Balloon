#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"
#include "Resources.h"
#include "Camera.h"
#include "TiledRender.h"
#include "BirdPoints.h"
#include "Hawk.h"

std::vector<BirdPoints*> pointBirds;
std::vector<Hawk*> hawks;
sf::Vector2f scale = { 10, 10 };

Camera camera(2000);
struct gameData {
	int totalPoints = 0;
	int availablePointBirds = 0;
	sf::Vector2f playerPosition = { 0 , 0 };
};



//you need to make an object of the struct to access the data within
gameData data;

void resetGame() {
	data.totalPoints = 0;
	for (BirdPoints* bp : pointBirds) {
		delete bp;
	}
	pointBirds.clear();
	for (Hawk* h : hawks) {
		delete h;
	}
	hawks.clear();
	data.availablePointBirds = 0;
	data.playerPosition = { 0, 0 };
	camera.position = data.playerPosition;
}

void spawnPointBirds(sf::Clock& clock, sf::Sprite player) {
	if ((clock.getElapsedTime().asSeconds() > 1 && data.availablePointBirds < 10 ) || pointBirds.size() == 0) {
		sf::Vector2f pos;
		pos.x = player.getPosition().x + (std::rand() % 2000 - 1000);
		pos.y = player.getPosition().y + (std::rand() % 2000 - 1000);
		int pointValue = rand() % 9 + 1;
		BirdPoints* pointBird;
		switch (pointValue) {
		//blank cases waterfall into the cases below them. Only stops if "break" is included
		case 2:
		case 4:
		case 7:
		case 8:
		case 1:
			pointBird = new BirdPoints(Resources::textures["pinkBird.png"], pos, scale, 1, 400);
			break;
		case 6:
		case 9:
		case 3:
			pointBird = new BirdPoints(Resources::textures["blueBird.png"], pos, scale, 3, 600);
			break;
		case 5:
			pointBird = new BirdPoints(Resources::textures["yellowBird.png"], pos, scale, 5, 800);
			break;
		default:
			pointBird = new BirdPoints(Resources::textures["pinkBird.png"], pos, scale, 1, 400);
		};
		//BirdPoints* pointBird = new BirdPoints(Resources::textures["Bird.png"], pos, scale, rand() % 4 + 1);
		data.availablePointBirds++;
		pointBirds.push_back(pointBird);
		clock.restart();
	}
}

int main()
{

	sf::Clock frameClock;
	sf::Clock pointClock;

	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ 1900, 1800 }), "Hot Air Balloon :3");

	sf::Vector2f centerTile = { 100, 100 };
	
	sf::CircleShape circle;

	TiledRender tiledRenderer;

	circle.setFillColor(sf::Color::Blue);
	circle.setRadius(10);
	circle.setScale(scale);

	// file.path().extension() == ".ttf")

	//sets up loading images, you dont need to retype above for every image
	for (const auto& file : std::filesystem::directory_iterator("./resources/textures"))
	{
		if (file.is_regular_file() && (file.path().extension() == ".png")) {
			if (!Resources::textures[file.path().filename().string()].loadFromFile(file.path().string()))
				std::abort();
		}
		else if (file.is_regular_file() && (file.path().extension() == ".ttf")) {
			if (!Resources::fonts[file.path().filename().string()].openFromFile(file.path().string()))
				std::abort();
			}
	}

	

	//if (!balloonTexture.loadFromFile("resources/textures/Balloon.png"))
	//{
	//	std::cout << "Could not load file resources/textures/Balloon.png" << std::endl;
	//	return -1;
	//} 

	sf::Sprite background(Resources::textures["TiledClouds.png"]);
	background.setOrigin({ 150, 150 });
	background.setPosition({ 0, 0 });
	background.setScale({ 10, 10 });

	sf::Sprite balloon(Resources::textures["Balloon.png"]);
	balloon.setOrigin({ 6, 8 });
	balloon.setPosition(data.playerPosition);
	balloon.setScale({ 10, 10 });

	Hawk* hawk = new Hawk(Resources::textures["BirdFlying1.png"], { 800, 0 }, scale, 50, balloon);
	hawks.push_back(hawk);
	//point bird vector

	//BirdPoints* pointBird = new BirdPoints(Resources::textures["Bird.png"], { 100, 100 }, scale, 1);
	//pointBirds.push_back(pointBird);

	float playerSpeed = 1000;


	//balloon.setTextureRect(sf::IntRect({ 0,0 }, { 11 ,16 })); (You don't need this unless working from an image with multiple sprites

	

	camera.position = { 0, 0 };
	sf::Text pointUI(Resources::fonts["StackedPixel.ttf"]);
	pointUI.setScale({3, 3});
	

	while (window->isOpen())
	{
		//clock to standardized player speed across frame rates
		float deltaTime = frameClock.getElapsedTime().asSeconds();
		frameClock.restart();

		

		while (const std::optional event = window->pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window->close();
		}
		

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

		data.playerPosition += velocity;
		balloon.setPosition(data.playerPosition);

		spawnPointBirds(pointClock, balloon);
		

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

		//set window and camera view
		window->clear();
		window->setView(camera.GetView(window->getSize()));
		//window->draw(background);

		//render background
		tiledRenderer.updateCenterTile(window, centerTile, camera, Resources::textures["TiledClouds.png"].getSize().x, scale.x);
		tiledRenderer.render(window, Resources::textures["TiledClouds.png"],
			Resources::textures["TiledClouds.png"].getSize().x, scale, centerTile);
		circle.setPosition(centerTile);
		
		std::vector<BirdPoints*> tempBirds;

		int despawnRadius = 3500;
		//Render all point birds
		for (BirdPoints* bp : pointBirds)
		{
			bp->move(deltaTime);

			if (bp->checkPlayerCollision(balloon)) {
				data.totalPoints += bp->pointValue;
				delete(bp);
				data.availablePointBirds--;
			//despawns birds. make bool function later
			} else if (balloon.getPosition().x - bp->position.x > despawnRadius
				|| balloon.getPosition().y - bp->position.y > despawnRadius
				|| bp->position.x - balloon.getPosition().x > despawnRadius
				|| bp->position.y - balloon.getPosition().y  > despawnRadius) {

				delete(bp);
				data.availablePointBirds--;
			} else {
				tempBirds.push_back(bp);
				bp->render(window);
			}

			
			//float speed = 500 * deltaTime;
			
			

		}

		std::string pointMessage = "Points: " + std::to_string(data.totalPoints);
		sf::View cameraView = camera.GetView(window->getSize());
		pointUI.setPosition({ (cameraView.getCenter().x - cameraView.getSize().x / 2) + 30,
			cameraView.getCenter().y - cameraView.getSize().y / 2 });

		pointUI.setString(pointMessage);
		pointUI.setOutlineColor(sf::Color::Black);
		pointUI.setOutlineThickness(1.5);
		//std::cout << "Points: " << data.totalPoints << std::endl;

		pointBirds = tempBirds;

		bool circleHit = false;

		//collision thing
		//.getGlobal position and .findIntersection (takes a rect)
		for (Hawk* h : hawks) {
			h->move(deltaTime);
			h->render(window);
			if (h->checkCollision(balloon)) {
				resetGame();
			}
		}
		
		window->draw(pointUI);
		//hawk.render(window);
		//render player
		window->draw(balloon);
		window->display();

	}

	delete window;
	return 0;

}
