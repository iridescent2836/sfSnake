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

	void generateFruit(sf::Color color);
	void renderGrid(sf::RenderWindow& window);
	void renderBackground(sf::RenderWindow& window);
	void initializeFruits();
	void initBalls();
	void updateBalls();
	void renderBalls(sf::RenderWindow& window);
	void fireballMove(sf::Time delta);


private:
	Snake snake_;
	std::vector<Fruit> fruit_;

	sf::Text scores_;
	sf::Font font_;

	int numberOfFruits_;

	sf::RectangleShape gridVline;   // Verical line
    sf::RectangleShape gridHline;   // Horizon Line

	sf::Sprite background_;
	sf::Texture backgroundTexture_;

	std::vector<sf::CircleShape> snowballs_;
	sf::Texture snowballTexture_;

	std::vector<sf::CircleShape> fireballs_;
	sf::Texture fireballTexture_;

	sf::CircleShape lavaball_;
	sf::Texture lavaballTexture_;

	const float ballRadius_ = 15.f;
	const int ballNum_ = 3;

	static std::default_random_engine engine;
	static std::uniform_int_distribution<int> xDistribution;
	static std::uniform_int_distribution<int> yDistribution;
};
}

#endif