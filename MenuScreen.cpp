#include <SFML/Graphics.hpp>

#include <memory>
#include <iostream>

#include "GameScreen.h"
#include "MenuScreen.h"
#include "Game.h"

using namespace sfSnake;

MenuScreen::MenuScreen() : backGroundButton1_(sf::Vector2f(Game::Width / 10, Game::Height / 10)), 

						   gridButton_(sf::Vector2f(Game::Width / 11, Game::Height / 20))
{
	//set font
	font_.loadFromFile("Fonts/game_over.ttf");
	// set up text
	text_.setFont(font_);
	text_.setString(
		"\n\n\n\n\nPress [SPACE] to play"
		"\n\nPress [ESC] to quit"
		"\n\nPress [enter] to change game level");
	text_.setFillColor(sf::Color::White);
	text_.setOutlineColor(sf::Color::Black);
	text_.setOutlineThickness(2);
	text_.setCharacterSize(Game::Width / 25);
	text_.setStyle(sf::Text::Bold);

	// set up snake text
	snakeText_.setFont(font_);
	snakeText_.setString("Snake!");
	snakeText_.setFillColor(sf::Color::White);
	snakeText_.setOutlineColor(sf::Color::Black);
	snakeText_.setOutlineThickness(2);
	snakeText_.setCharacterSize(Game::Width / 10);
	snakeText_.setStyle(sf::Text::Bold);

	sf::FloatRect textBounds = text_.getLocalBounds();
	text_.setOrigin(textBounds.left + textBounds.width / 2,
		textBounds.top + textBounds.height / 2);
	text_.setPosition(Game::Width / 2, Game::Height / 2);

	sf::FloatRect snakeTextBounds = snakeText_.getLocalBounds();
	snakeText_.setOrigin(snakeTextBounds.left + snakeTextBounds.width / 2,
		snakeTextBounds.top + snakeTextBounds.height / 2);
	snakeText_.setPosition(Game::Width / 2, Game::Height / 4);

	// set up background
	if (!backgroundTexture_.loadFromFile("Backgrounds/background1.png")){
		std::cerr << "Failed to load background texture" << std::endl;
	}
	backgroundSprite_.setTexture(backgroundTexture_);
	backgroundSprite_.setScale(Game::Width / float(backgroundTexture_.getSize().x), Game::Height / float(backgroundTexture_.getSize().y));
	backgroundSprite_.setPosition(0, 0);
	initButtons();
}

void MenuScreen::initButtons(){
	if(Game::backGround == 0)
		backGroundButton1Texture_.loadFromFile("Backgrounds/white.png");
	else if(Game::backGround == 1)
		backGroundButton1Texture_.loadFromFile("Backgrounds/black.png");
	else if(Game::backGround == 2)
		backGroundButton1Texture_.loadFromFile("Backgrounds/orange.png");
	else if(Game::backGround == 3)
		backGroundButton1Texture_.loadFromFile("Backgrounds/snow.png");


	sf::Vector2f buttonSize(Game::Width / 20, Game::Height / 20);

	backGroundButton1_.setTexture(&backGroundButton1Texture_);
	backGroundButton1_.setPosition(buttonSize.x,buttonSize.y);	
	backGroundButton1_.setOutlineColor(sf::Color::Yellow);
	backGroundButton1_.setOutlineThickness(2);
	



	// gridButton_.setTexture(&gridButtonTexture_);
	gridButton_.setFillColor(sf::Color::Transparent);
	gridButton_.setPosition(buttonSize.x   , buttonSize.y  * 4);	
	gridButton_.setOutlineColor(sf::Color::Transparent);
	gridButton_.setOutlineThickness(2);

	gridText_.setFont(font_);
	gridText_.setString("Grid?");
	gridText_.setFillColor(sf::Color::White);
	gridText_.setOutlineColor(sf::Color::Black);
	gridText_.setOutlineThickness(2);
	gridText_.setCharacterSize(Game::Width / 30);
	gridText_.setStyle(sf::Text::Bold);
	gridText_.setPosition(gridButton_.getPosition());


}

void MenuScreen::handleInput(sf::RenderWindow& window)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		Game::Screen = std::make_shared<GameScreen>();
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		window.close();
	checkButtonPress(window);
}

void MenuScreen::checkButtonPress(sf::RenderWindow& window){
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	int backGournd = Game::backGround;
	if (backGroundButton1_.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && backGournd == 3)
	{
		backGroundButton1Texture_.loadFromFile("Backgrounds/white.png");
		// backGroundButton1_.setTexture(&backGroundButton1Texture_);
		Game::backGround = 0;
	}
	else if (backGroundButton1_.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && backGournd == 0)
	{
		backGroundButton1Texture_.loadFromFile("Backgrounds/black.png");
		// backGroundButton1_.setTexture(&backGroundButton2Texture_);
		Game::backGround = 1;
	}
	else if (backGroundButton1_.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && backGournd == 1)
	{
		backGroundButton1Texture_.loadFromFile("Backgrounds/orange.png");
		// backGroundButton1_.setTexture(&backGroundButton3Texture_);
		Game::backGround = 2;
	}
	else if (backGroundButton1_.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && backGournd == 2)
	{
		backGroundButton1Texture_.loadFromFile("Backgrounds/snow.png");
		// backGroundButton1_.setTexture(&backGroundButton4Texture_);
		Game::backGround = 3;
	}

	///////////////Grid Button/////////////////
	//BUGGGGGGGG!!!!!
	//you CANNOT use 
	//if(gridText_.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && Game::isGrid == false)
	//or the gamescreen cannot be rendered
	//so I use the following code instead
	if(gridButton_.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && Game::isGrid == false)
	{
		gridText_.setString("Grid!");
		gridText_.setFillColor(sf::Color::Black);
		gridText_.setOutlineColor(sf::Color::White);

		Game::isGrid = true;
	}
	else if(gridButton_.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && Game::isGrid == true)
	{
		gridText_.setString("Grid?");
		gridText_.setFillColor(sf::Color::White);
		gridText_.setOutlineColor(sf::Color::Black);
		Game::isGrid = false;
	}
	
	
}

void MenuScreen::update(sf::Time delta)
{
	static bool movingLeft = false;
	static bool movingRight = true;

	if (movingRight)
	{
		snakeText_.rotate(delta.asSeconds());

		if (static_cast<int>(snakeText_.getRotation()) == 10)
		{
			movingRight = false;
			movingLeft = true;
		}
	}

	if (movingLeft)
	{
		snakeText_.rotate(-delta.asSeconds());

		if (static_cast<int>(snakeText_.getRotation()) == (360 - 10))
		{
			movingLeft = false;
			movingRight = true;
		}
	}

}



void MenuScreen::render(sf::RenderWindow& window)
{
	window.draw(backgroundSprite_);
	window.draw(text_);
	window.draw(snakeText_);

	window.draw(backGroundButton1_);

	window.draw(gridButton_);
	window.draw(gridText_);
}