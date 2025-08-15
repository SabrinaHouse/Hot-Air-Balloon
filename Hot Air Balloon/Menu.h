#pragma once
#include <SFML/Graphics.hpp>
#include "Resources.h"
#include "Camera.h"
#include <iostream>

#define MAX_NUMBER_OF_ITEMS 3

class Menu
{
public:
	
	Menu(Camera camera, sf::RenderWindow* window);
	~Menu();

	void draw(sf::RenderWindow* window);
	void updatePosition(Camera camera, sf::RenderWindow* window);
	void moveUp();
	void moveDown();
	void updateHighScore(int newScore);
	const int getPressedItem() 
	{ return selectedItemIndex; }
	
private:
	int selectedItemIndex;
	sf::Font font;
	sf::Text* menu[MAX_NUMBER_OF_ITEMS];


};

