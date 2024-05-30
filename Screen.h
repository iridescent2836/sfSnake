#ifndef SCREEN_H
#define SCREEN_H

#include <SFML/Graphics.hpp>

class Screen
{
public:
	//pure virual function, must be overridden
	virtual void handleInput(sf::RenderWindow& window) = 0;
	virtual void update(sf::Time delta) = 0;
	virtual void render(sf::RenderWindow& window) = 0;

};

#endif