#include <SFML/Graphics.hpp>

#include <random>
#include <memory>

#include "GameScreen.h"
#include "GameOverScreen.h"
#include "Game.h"

using namespace sfSnake;

GameScreen::GameScreen() : snake_()
{

}

void GameScreen::handleInput(sf::RenderWindow& window)
{
	snake_.handleInput(window);
}

void GameScreen::update(sf::Time delta)
{

	static int numberOfFruit = 10;
	int deltaFruit = numberOfFruit - fruit_.size();
	if (deltaFruit > 0)
		generateFruit(deltaFruit);

	snake_.update(delta);
	snake_.checkFruitCollisions(fruit_);

	if (snake_.hitSelf())
		Game::Screen = std::make_shared<GameOverScreen>(snake_.getSize());
}

void GameScreen::render(sf::RenderWindow& window)
{
	snake_.render(window);

	for (auto fruit : fruit_)
		fruit.render(window);
}

std::default_random_engine GameScreen::engine(time(nullptr));
std::uniform_int_distribution<int> GameScreen::xDistribution(0, Game::Width - SnakeNode::Width);
std::uniform_int_distribution<int> GameScreen::yDistribution(0, Game::Height - SnakeNode::Height);

void GameScreen::generateFruit(int numberOfFruits)
{
	// static std::default_random_engine engine;
	// engine.seed(time(NULL));
	// static std::uniform_int_distribution<int> xDistribution(0, Game::Width - SnakeNode::Width);
	// static std::uniform_int_distribution<int> yDistribution(0, Game::Height - SnakeNode::Height);

	// fruit_.push_back(Fruit(sf::Vector2f(xDistribution(engine), yDistribution(engine))));
	for(int i = 0; i < numberOfFruits;++i){
		int x = xDistribution(engine);
		int y = yDistribution(engine);
		fruit_.push_back(Fruit(sf::Vector2f(x, y)));
	}

}

