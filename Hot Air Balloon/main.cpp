#include <SFML/Graphics.hpp>
#include <iostream>
#include "Resources.h"
#include "Camera.h"
#include "TiledRender.h"
#include "BirdPoints.h"
#include "Hawk.h"
#include "Menu.h"

//starting data
std::vector<BirdPoints*> pointBirds;
std::vector<Hawk*> hawks;
sf::Vector2f scale = { 10, 10 };
float spawnRadius = 100;
int hawkDespawnRadius = 2000;
float playerSpeed = 1000;
int despawnRadius = 3500;
float hawkTime = 9;
int hawkSpeed = 1000;
std::string highScoreMessage = "High Score: 0";

//make window and camera
sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ 1900, 1800 }), "Hot Air Balloon :3");
Camera camera(2000);


bool inMenu = true;

//data that will change throughout the game
struct gameData {
	int totalPoints = 0;
	int availablePointBirds = 0;
	sf::Vector2f playerPosition = { 0 , 0 };
	int maxHawks = 4;
	int highScore = 0;
};


//you need to make an object of the struct to access the data within
gameData data;

//resetings all data changed during game
void resetGame() {
	if (data.totalPoints > data.highScore) {
		data.highScore = data.totalPoints;
	}
	data.totalPoints = 0;
	for (BirdPoints* bp : pointBirds) {
		delete bp;
	}
	pointBirds.clear();
	for (Hawk* h : hawks) {
		delete h;
	}
	hawks.clear();
	hawkTime = 9;
	hawkSpeed = 1000;
	data.availablePointBirds = 0;
	data.playerPosition = { 0, 0 };
	camera.position = data.playerPosition;
	inMenu = true;
}

//hawks randomly spawn on the edges of your screen
void spawnHawks(sf::Clock& clock, sf::Sprite player) {
	if (hawks.size() >= data.maxHawks) {
		return;
	}
	if (clock.getElapsedTime().asSeconds() > hawkTime) {
		sf::Vector2f pos;

		int side = rand() % 4;
		sf::View view = camera.GetView(window->getSize());
		int xMin = (int)( view.getCenter().x - (view.getSize().x / 2.0));
		int xMax = (int)( view.getCenter().x + (view.getSize().x / 2.0));
		int yMin = (int)( view.getCenter().y - (view.getSize().y / 2.0));
		int yMax = (int)( view.getCenter().y + (view.getSize().y / 2.0));

		switch (side)
		{
		case 0: //left
			pos.x = xMin;
			pos.y = rand() % (yMax - yMin) + yMin;
			break;
		case 1: //right
			pos.x = xMax;
			pos.y = rand() % (yMax - yMin) + yMin;
			break;
		case 2: // top
			pos.y = yMin;
			pos.x = rand() % (xMax - xMin) + xMin;
			break;
		case 3: // bottom
			pos.y = yMax;
			pos.x = rand() % (xMax - xMin) + xMin;
			break;
		}

		Hawk* hawk = new Hawk(Resources::textures["BirdFlying1.png"], pos, scale, hawkSpeed , player);
		hawks.push_back(hawk);
		clock.restart();
	}
}

//birds spawn randomly around the player
void spawnPointBirds(sf::Clock& clock, sf::Sprite player) {
	if ((clock.getElapsedTime().asSeconds() > 1 && pointBirds.size() < 10) || pointBirds.size() == 0) {
		sf::Vector2f pos;
		pos.x = player.getPosition().x + (std::rand() % 2000 - 1000);
		pos.y = player.getPosition().y + (std::rand() % 2000 - 1000);
		if ((pos.x >= player.getPosition().x + spawnRadius && pos.y >= player.getPosition().y + spawnRadius)) {
			pos.x += spawnRadius;
		}
		else if (pos.x <= player.getPosition().x - spawnRadius && pos.y <= player.getPosition().y - spawnRadius) {
			pos.x -= spawnRadius;
		}

		int pointValue = rand() % 9 + 1;
		BirdPoints* pointBird;
		switch (pointValue)
		{
			//blank cases waterfall into the cases below them. Only stops if "break" is included
			//pink birds are most common, worth 1 point
		case 2:
		case 4:
		case 7:
		case 8:
		case 1:
			pointBird = new BirdPoints(Resources::textures["pinkBird.png"], pos, scale, 1, 400);
			break;
			//blue bird is second common, worth 3 points
		case 6:
		case 9:
		case 3:
			pointBird = new BirdPoints(Resources::textures["blueBird.png"], pos, scale, 3, 600);
			break;
		case 5:
			//yellow bird is least common, worth 5 points
			pointBird = new BirdPoints(Resources::textures["yellowBird.png"], pos, scale, 5, 800);
			break;
		default:
			//incase switch case fails, spawn most common bird (pink)
			pointBird = new BirdPoints(Resources::textures["pinkBird.png"], pos, scale, 1, 400);
		}
		data.availablePointBirds++;
		pointBirds.push_back(pointBird);
		clock.restart();
	}	
}

int main()
{

	sf::Clock frameClock;
	sf::Clock pointClock;
	sf::Clock hawkClock;

	sf::Vector2f centerTile = { 100, 100 };

	TiledRender tiledRenderer;


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

	camera.position = { 0, 0 };

	Menu menu(camera, window);

	sf::Sprite background(Resources::textures["TiledClouds.png"]);
	background.setOrigin({ 150, 150 });
	background.setPosition({ 0, 0 });
	background.setScale({ 10, 10 });

	sf::Sprite balloon(Resources::textures["Balloon.png"]);
	balloon.setOrigin({ 6, 8 });
	balloon.setPosition(data.playerPosition);
	balloon.setScale({ 10, 10 });

	
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
			//menu navigation
			if (event->is<sf::Event::KeyReleased>()) {
				auto key = event->getIf<sf::Event::KeyReleased>();
				if (key->code == sf::Keyboard::Key::Up || key->code == sf::Keyboard::Key::W) { //UP
					menu.moveUp();
				}
				if (key->code == sf::Keyboard::Key::Down || key->code == sf::Keyboard::Key::S) { //DOWN
					menu.moveDown();
				}
				if (key->code == sf::Keyboard::Key::Enter || key->code == sf::Keyboard::Key::Space) { //SELECT
					switch (menu.getPressedItem()) {
					case 0:
						inMenu = false;
						break;
					case 1:
						window->close();
						break;
					}
				}
				if (key->code == sf::Keyboard::Key::Escape) {
					if (data.totalPoints > data.highScore) {
						data.highScore = data.totalPoints;
					}
					menu.updateHighScore(data.highScore);
					inMenu = true;
				}
			}
		}
		
		if (!inMenu) {
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
			spawnHawks(hawkClock, balloon);


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

			//render background
			tiledRenderer.updateCenterTile(window, centerTile, camera, Resources::textures["TiledClouds.png"].getSize().x, scale.x);
			tiledRenderer.render(window, Resources::textures["TiledClouds.png"],
				Resources::textures["TiledClouds.png"].getSize().x, scale, centerTile);

			std::vector<BirdPoints*> tempBirds;

			//Render all point birds and check to make sure they have not gone too far
			for (BirdPoints* bp : pointBirds)
			{
				bp->move(deltaTime);

				if (bp->checkPlayerCollision(balloon)) {
					data.totalPoints += (int)bp->pointValue;
					//edit hawk speed and frequency based on points
					hawkSpeed = hawkSpeed + (bp->pointValue * 7);
					hawkTime = hawkTime - 0.005;
					delete(bp);
					data.availablePointBirds--;
				}
				else if (balloon.getPosition().x - bp->position.x > despawnRadius
					|| balloon.getPosition().y - bp->position.y > despawnRadius
					|| bp->position.x - balloon.getPosition().x > despawnRadius
					|| bp->position.y - balloon.getPosition().y > despawnRadius) {

					delete(bp);
					data.availablePointBirds--;
				}
				else {
					tempBirds.push_back(bp);
					bp->render(window);
				}

			}

			sf::View cameraView = camera.GetView(window->getSize());

			//highscore
			std::string pointMessage = "Points: " + std::to_string(data.totalPoints);
			pointUI.setPosition({ (cameraView.getCenter().x - cameraView.getSize().x / 2) + 30,
				cameraView.getCenter().y - cameraView.getSize().y / 2 });
			pointUI.setString(pointMessage);
			pointUI.setOutlineColor(sf::Color::Black);
			pointUI.setOutlineThickness(1.5);

			pointBirds = tempBirds;
			std::vector<Hawk*> tempHawks;

			//checking to make sure hawks have not hit player or gone too far
			for (Hawk* h : hawks) {
				h->move(deltaTime);

				if (h->checkCollision(balloon)) {
					resetGame();
					hawkClock.restart();
					break;
				}
				else if (h->despawnHawk(balloon, hawkDespawnRadius) && hawks.size() > 0) {
					auto it = std::find(hawks.begin(), hawks.end(), h);
					int index = std::distance(hawks.begin(), it);
					hawks.erase(hawks.begin() + index);
				}
				else {
					tempHawks.push_back(h);
					h->render(window);
				}
			}

			window->draw(pointUI);
			menu.updateHighScore(data.highScore);
			//render player
			window->draw(balloon);
			
		}
	else {
		//starting/pause menu
		window->clear();
		tiledRenderer.updateCenterTile(window, centerTile, camera, Resources::textures["TiledClouds.png"].getSize().x, scale.x);
		tiledRenderer.render(window, Resources::textures["TiledClouds.png"],Resources::textures["TiledClouds.png"].getSize().x, scale, centerTile);
		window->setView(camera.GetView(window->getSize()));
		menu.updatePosition(camera, window);
		menu.draw(window);
	}
		window->display();
	}

	delete window;
	return 0;
}
