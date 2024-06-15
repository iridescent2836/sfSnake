#ifndef GAME_OVER_SCREEN_H
#define GAME_OVER_SCREEN_H

#include <SFML/Graphics.hpp>

#include "Screen.h"

namespace sfSnake
{
class GameOverScreen : public Screen
{
public:
	GameOverScreen(std::size_t score);

	void handleInput(sf::RenderWindow& window) override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;

	void checkButtonPress(sf::RenderWindow& window);


private:
	sf::Font font_;
	sf::Text text_;

	unsigned score_;

	sf::Texture backgroundTexture_;
	sf::Sprite backgroundSprite_;



};
}

#endif