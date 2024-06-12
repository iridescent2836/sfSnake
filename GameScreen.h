#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <SFML/Graphics.hpp>

#include <random>

#include "Screen.h"
#include "Snake.h"
#include "Fruit.h"

namespace sfSnake
{
class GameScreen : public Screen
{
public:
	GameScreen();

	void handleInput(sf::RenderWindow& window) override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;

	void generateFruit(int numberOfFruits);

private:
	Snake snake_;
	std::vector<Fruit> fruit_;

	sf::Text scores_;
	sf::Font font_;


	sf::Sprite background_;
	sf::Texture backgroundTexture_;

	static std::default_random_engine engine;
	static std::uniform_int_distribution<int> xDistribution;
	static std::uniform_int_distribution<int> yDistribution;
};
}

#endif