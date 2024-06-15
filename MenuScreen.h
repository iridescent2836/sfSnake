#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include <SFML/Graphics.hpp>

#include "Screen.h"

namespace sfSnake
{
class MenuScreen : public Screen
{
public:
	MenuScreen();

	void handleInput(sf::RenderWindow& window) override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;
	void checkButtonPress(sf::RenderWindow& window);
	void initButtons();

private:
	sf::Font font_;
	sf::Text snakeText_;
	sf::Text text_;
	sf::Texture backgroundTexture_;
	sf::Sprite backgroundSprite_;

	sf::RectangleShape backGroundButton1_;
	// sf::RectangleShape backGroundButton3_;
	// sf::RectangleShape backGroundButton4_;
	sf::RectangleShape gridButton_;

	sf::Texture backGroundButton1Texture_;
	
	//just to mark the bounds of gridtext
	sf::Texture gridButtonTexture_;

	sf::Text gridText_;

	bool isGameScreenBotton1Pressed_ = false;
	bool isGameScreenBotton2Pressed_ = false;
	bool isSnakeButton1Pressed_ = false;
	bool isSnakeButton2Pressed_ = false;
	
};
}

#endif