#include <SFML/Graphics.hpp>

#include <random>
#include <string>
#include <memory>
#include <iostream>

#include "GameScreen.h"
#include "GameOverScreen.h"
#include "Game.h"

using namespace sfSnake;


GameScreen::GameScreen() : snake_(),numberOfFruits_(10),gridVline(sf::Vector2f(Game::Width/ 500.0f, Game::Height)),
      gridHline(sf::Vector2f(Game::Width, Game::Width / 500.0f))
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
	if(Game::backGround == 0)
		backgroundTexture_.loadFromFile("Backgrounds/white.png");
	else if(Game::backGround == 1)
		backgroundTexture_.loadFromFile("Backgrounds/black.png");
	else if(Game::backGround == 2)
		backgroundTexture_.loadFromFile("Backgrounds/orange.png");
	else if(Game::backGround == 3)
		backgroundTexture_.loadFromFile("Backgrounds/snow.png");
	background_.setTexture(backgroundTexture_);
	background_.setPosition(0,0);

	//set snowball and fireball textures
	snowballTexture_.loadFromFile("Elements/snowball.png");
	fireballTexture_.loadFromFile("Elements/fireball.png");

	initializeFruits();	
	initBalls();
}

void GameScreen::initBalls(){
	for(int i = 0; i < ballNum_; ++i){
		sf::CircleShape snowball(ballRadius_);
		sf::CircleShape fireball(ballRadius_);
		snowball.setTexture(&snowballTexture_);
		fireball.setTexture(&fireballTexture_);

		snowball.setOrigin(ballRadius_, ballRadius_);
		fireball.setOrigin(ballRadius_, ballRadius_);

		snowball.setPosition(xDistribution(engine), yDistribution(engine));
		fireball.setPosition(xDistribution(engine), yDistribution(engine));

		snowballs_.push_back(snowball);
		fireballs_.push_back(fireball);
	}
}

void GameScreen::initializeFruits(){
	// initialize black and brown fruits
	for(int i = 0; i < numberOfFruits_*0.25; ++i){
		Fruit fruit(sf::Vector2f(xDistribution(engine), yDistribution(engine)), xDistribution(engine) % 2);
		fruit_.push_back(fruit);
	}
	// initialize red, blue, and green fruits
	for(int i = 0; i < numberOfFruits_*0.75; ++i){
		Fruit fruit(sf::Vector2f(xDistribution(engine), yDistribution(engine)), xDistribution(engine) % 3 + 2);
		fruit_.push_back(fruit);
	}
}

void GameScreen::handleInput(sf::RenderWindow& window)
{
	snake_.handleInput(window);
}

void GameScreen::update(sf::Time delta)
{


	static bool is_egg = true;
	//an egg, 242 = 120 + 122, ascii
	if(snake_.getScores() == 242 && is_egg){
		Fruit fruit(sf::Vector2f(xDistribution(engine), yDistribution(engine)), 2);
		fruit.setColor(sf::Color::Yellow);
		fruit_.push_back(fruit);
		is_egg = false;
	}

	snake_.update(delta);	//move and check collison
	sf::Color color = snake_.checkFruitCollisions(fruit_);	//check fruit collision and grow if needed
	snake_.checkBallCollisions(snowballs_, fireballs_);	//check ball collision

	updateBalls();

	generateFruit(color);	//generate new fruit if needed



	//goto gameover screen
	if (snake_.hitSelf())
		Game::Screen = std::make_shared<GameOverScreen>(snake_.getScores());

	scores_.setString("SCORE:" + std::to_string(snake_.getScores()));
	
}

void GameScreen::updateBalls(){
	if(snowballs_.size() < ballNum_){
		int delta = ballNum_ - snowballs_.size();
		for(int i = 0; i < delta; ++i){
			sf::CircleShape snowball(ballRadius_);
			snowball.setTexture(&snowballTexture_);
			snowball.setOrigin(ballRadius_, ballRadius_);
			snowball.setPosition(xDistribution(engine), yDistribution(engine));
			snowballs_.push_back(snowball);
		}
	}
	if(fireballs_.size() < ballNum_){

		int delta = ballNum_ - fireballs_.size();
		for(int i = 0; i < delta; ++i){
			sf::CircleShape fireball(ballRadius_);
			fireball.setTexture(&fireballTexture_);
			fireball.setOrigin(ballRadius_, ballRadius_);
			fireball.setPosition(xDistribution(engine), yDistribution(engine));
			fireballs_.push_back(fireball);
		}
	}
}


void GameScreen::renderGrid(sf::RenderWindow& window){

	if(Game::backGround == 0 || Game::backGround == 3){
		gridVline.setFillColor({205,221,253});
    	gridHline.setFillColor({205,221,253});
	}
	else if(Game::backGround == 2){
		gridVline.setFillColor({219,163,105});
    	gridHline.setFillColor({219,163,105});
	}
	else{
		gridVline.setFillColor({35,33,52});
    	gridHline.setFillColor({35,33,52});
	}

    static float gridSpace = Game::Width / 50.0f;
    for (float i = 0.f; i < Game::Width; i += gridSpace)
    {
        gridVline.setPosition(i, 0);
        window.draw(gridVline);
    }

    for (float i = 0.f; i < Game::Height; i += gridSpace)
    {
        gridHline.setPosition(0, i);
        window.draw(gridHline);
    }
}

void GameScreen::renderBackground(sf::RenderWindow& window){
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

}


void GameScreen::renderBalls(sf::RenderWindow& window){
	for(auto snowball : snowballs_){
		window.draw(snowball);
	}
	for(auto fireball : fireballs_){
		window.draw(fireball);
	}
}

//no background render, only render the snake and fruit
void GameScreen::render(sf::RenderWindow& window)
{
	renderBackground(window);

	if(Game::isGrid)
		renderGrid(window);

	snake_.render(window);

	for (auto fruit : fruit_)
		fruit.render(window);

	renderBalls(window);

	window.draw(scores_);

}

std::default_random_engine GameScreen::engine(time(nullptr));
std::uniform_int_distribution<int> GameScreen::xDistribution(0, Game::Width - SnakeNode::Width);
std::uniform_int_distribution<int> GameScreen::yDistribution(0, Game::Height - SnakeNode::Height);

void GameScreen::generateFruit(sf::Color color)
{
	//generate new fruit if needed
	if (color!= sf::Color::Transparent){
		// radomly generate red, blue, green fruit
		if(color == sf::Color::Red || color == sf::Color::Blue || color == sf::Color::Green){
			Fruit fruit(sf::Vector2f(xDistribution(engine), yDistribution(engine)),
						xDistribution(engine) % 3 + 2);
			fruit_.push_back(fruit);
		}
		// randomly generate brown and black fruit
		else{
			Fruit fruit(sf::Vector2f(xDistribution(engine), yDistribution(engine)),
						xDistribution(engine) % 2);
			fruit_.push_back(fruit);
		}
	}

}

