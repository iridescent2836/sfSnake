#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <memory>

#include "Screen.h"

namespace sfSnake
{
class Game
{
public:
	Game();

	void run();

	void handleInput();
	void update(sf::Time delta);
	void render();

	static const int Width = 1280;
	static const int Height = 960;

	static std::shared_ptr<Screen> Screen;


	static int backGround; //1:black, 0:white, 2:orange, 3:snow
	static bool isGrid;  //0:no grid, 1:grid

private:
	sf::RenderWindow window_;
	sf::Music bgMusic_;
	
	static const sf::Time TimePerFrame;
};
}


#endif