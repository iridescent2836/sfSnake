#include <SFML/Graphics.hpp>

#include <random>
#include <string>
#include <memory>
#include <iostream>

#include "GameScreen.h"
#include "GameOverScreen.h"
#include "Game.h"

using namespace sfSnake;

GameScreen::GameScreen() : snake_()
{
	font_.loadFromFile("Fonts/game_over.ttf");

	scores_.setFont(font_);
	scores_.setString("SCORE:" + std::to_string(snake_.getScores()));
	scores_.setFillColor(sf::Color::White);
	scores_.setOutlineColor(sf::Color::Black);
	scores_.setOutlineThickness(2.0f);
	scores_.setStyle(sf::Text::Bold);
	// std::cout << (Game::Width - scores_.getString().getSize()) / 2.0f << std::endl;
	scores_.setCharacterSize(Game::Width / 25.0f);
	scores_.setPosition((Game::Width - scores_.getString().getSize() * scores_.getCharacterSize()) / 2.0f , Game::Height / 20.0f);

	//set background
	backgroundTexture_.loadFromFile("Backgrounds/grid2.png");
	background_.setTexture(backgroundTexture_);
	background_.setPosition(0,0);
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
	static bool is_egg = true;
	//an egg, 242 = 120 + 122, ascii
	if(snake_.getScores() == 242 && is_egg){
		generateFruit(1);
		fruit_[fruit_.size() - 1].setColor(sf::Color::Yellow);
		is_egg = false;
	}

	snake_.update(delta);	//move and check collison
	snake_.checkFruitCollisions(fruit_);	//check fruit collision and grow if needed

	//goto gameover screen
	if (snake_.hitSelf())
		Game::Screen = std::make_shared<GameOverScreen>(snake_.getScores());

	scores_.setString("SCORE:" + std::to_string(snake_.getScores()));
	
}

//no background render, only render the snake and fruit
void GameScreen::render(sf::RenderWindow& window)
{
	// draw background
	int backgroundWidth = backgroundTexture_.getSize().x;
	int backgroundHeight = backgroundTexture_.getSize().y;

	int numCols = Game::Width / backgroundWidth + 1;
	int numRows = Game::Height / backgroundHeight + 1;

	for (int i = 0; i < numCols; ++i)
	{
		for (int j = 0; j < numRows; ++j)	//draw background
		{
			
			background_.setPosition(i * backgroundWidth, j * backgroundHeight);
			window.draw(background_);
		}
	}
	// window.draw(background_);


	snake_.render(window);

	for (auto fruit : fruit_)
		fruit.render(window);

	window.draw(scores_);

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
		fruit_.push_back(Fruit(sf::Vector2f(x, y), x % 3));
		// const sf::Color color = sf::Color::Blue;
		//  fruit_.end()->setColor(color);
		// fruit_[fruit_.size()-1].setColor(sf::Color::Red);
		
	}

}

