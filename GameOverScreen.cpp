#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>

#include "Game.h"
#include "GameScreen.h"
#include "GameOverScreen.h"

using namespace sfSnake;

GameOverScreen::GameOverScreen(std::size_t score) : score_(score),
													backGroundButton1_(sf::Vector2f(Game::Width / 20, Game::Height / 20)), 
													backGroundButton2_(sf::Vector2f(Game::Width / 20, Game::Height / 20)),
													backGroundButton3_(sf::Vector2f(Game::Width / 20, Game::Height / 20)), 
													backGroundButton4_(sf::Vector2f(Game::Width / 20, Game::Height / 20)), 
													gridButton_(sf::Vector2f(Game::Width / 11, Game::Height / 20))
{
	font_.loadFromFile("Fonts/game_over.ttf");
	text_.setFont(font_);
	text_.setString("Your score: " + std::to_string(score) + "!"
		"\n\nPress [SPACE] to retry"
		"\n\nPress [ESC] to quit");
	text_.setFillColor(sf::Color::White);
	text_.setCharacterSize(Game::Width / 20);
	text_.setOutlineColor(sf::Color::Black);
	text_.setOutlineThickness(2);
	text_.setStyle(sf::Text::Bold);

	sf::FloatRect textBounds = text_.getLocalBounds();
	text_.setOrigin(textBounds.left + textBounds.width / 2,
		textBounds.top + textBounds.height / 2);
	text_.setPosition(Game::Width / 2, Game::Height / 2);

	if (!backgroundTexture_.loadFromFile("Backgrounds/background1.png")){
		std::cerr << "Failed to load background texture" << std::endl;
	}
	backgroundSprite_.setTexture(backgroundTexture_);
	backgroundSprite_.setScale(Game::Width / float(backgroundTexture_.getSize().x), Game::Height / float(backgroundTexture_.getSize().y));
	backgroundSprite_.setPosition(0, 0);


	
	backGroundButton1Texture_.loadFromFile("Backgrounds/white.png");
	backGroundButton2Texture_.loadFromFile("Backgrounds/black.png");
	backGroundButton3Texture_.loadFromFile("Backgrounds/orange.png");
	backGroundButton4Texture_.loadFromFile("Backgrounds/snow.png");
	gridButtonTexture_.loadFromFile("Backgrounds/grid.png");

	sf::Vector2f buttonSize(Game::Width / 20, Game::Height / 20);

	backGroundButton1_.setTexture(&backGroundButton1Texture_);
	// backGroundButton1_.setScale(buttonSize.x / float(backGroundButton1Texture_.getSize().x), buttonSize.y / float(backGroundButton1Texture_.getSize().y));
	backGroundButton1_.setPosition(buttonSize.x,buttonSize.y);	
	backGroundButton1_.setOutlineThickness(2);
	
	
	backGroundButton2_.setTexture(&backGroundButton2Texture_);	
	// backGroundButton2_.setScale(buttonSize.x / float(backGroundButton2Texture_.getSize().x),buttonSize.y / float(backGroundButton2Texture_.getSize().y));
	backGroundButton2_.setPosition(buttonSize.x * 3, buttonSize.y );	
	backGroundButton2_.setOutlineThickness(2);

	backGroundButton3_.setTexture(&backGroundButton3Texture_);
	// backGroundButton3_.setScale(buttonSize.x / float(backGroundButton3Texture_.getSize().x), buttonSize.y / float(backGroundButton3Texture_.getSize().y));
	backGroundButton3_.setPosition(buttonSize.x , buttonSize.y  * 3);	
	backGroundButton3_.setOutlineThickness(2);

	backGroundButton4_.setTexture(&backGroundButton4Texture_);
	// backGroundButton4_.setScale(buttonSize.x / float(backGroundButton4Texture_.getSize().x), buttonSize.y / float(backGroundButton4Texture_.getSize().y));
	backGroundButton4_.setPosition(buttonSize.x  * 3, buttonSize.y  * 3);	
	backGroundButton4_.setOutlineThickness(2);

	if(Game::backGround == 0){
		backGroundButton1_.setOutlineColor(sf::Color::Yellow);
		backGroundButton2_.setOutlineColor(sf::Color::White);
		backGroundButton3_.setOutlineColor(sf::Color::White);
		backGroundButton4_.setOutlineColor(sf::Color::White);
	}
	else if(Game::backGround == 1){
		backGroundButton1_.setOutlineColor(sf::Color::White);
		backGroundButton2_.setOutlineColor(sf::Color::Yellow);
		backGroundButton3_.setOutlineColor(sf::Color::White);
		backGroundButton4_.setOutlineColor(sf::Color::White);
	}
	else if(Game::backGround == 2){
		backGroundButton1_.setOutlineColor(sf::Color::White);
		backGroundButton2_.setOutlineColor(sf::Color::White);
		backGroundButton3_.setOutlineColor(sf::Color::Yellow);
		backGroundButton4_.setOutlineColor(sf::Color::White);
	}
	else if(Game::backGround == 3){
		backGroundButton1_.setOutlineColor(sf::Color::White);
		backGroundButton2_.setOutlineColor(sf::Color::White);
		backGroundButton3_.setOutlineColor(sf::Color::White);
		backGroundButton4_.setOutlineColor(sf::Color::Yellow);
	}


	// gridButton_.setTexture(&gridButtonTexture_);
	gridButton_.setFillColor(sf::Color::Transparent);
	gridButton_.setPosition(buttonSize.x   , buttonSize.y  * 5);	
	if(Game::isGrid == true)
		gridButton_.setOutlineColor(sf::Color::Yellow);
	else
		gridButton_.setOutlineColor(sf::Color::Transparent);
	gridButton_.setOutlineThickness(2);

	gridText_.setFont(font_);
	gridText_.setString("Grid?");
	gridText_.setFillColor(sf::Color::White);
	gridText_.setOutlineColor(sf::Color::Black);
	gridText_.setOutlineThickness(2);
	gridText_.setCharacterSize(Game::Width / 30);
	gridText_.setStyle(sf::Text::Bold);
	gridText_.setPosition(buttonSize.x   , buttonSize.y  * 5);
}

void GameOverScreen::checkButtonPress(sf::RenderWindow& window){
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	if (backGroundButton1_.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		backGroundButton1_.setOutlineColor(sf::Color::Yellow);
		backGroundButton2_.setOutlineColor(sf::Color::White);
		backGroundButton3_.setOutlineColor(sf::Color::White);
		backGroundButton4_.setOutlineColor(sf::Color::White);
	
		Game::backGround = 0;
	}
	else if (backGroundButton2_.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		backGroundButton1_.setOutlineColor(sf::Color::White);
		backGroundButton2_.setOutlineColor(sf::Color::Yellow);
		backGroundButton3_.setOutlineColor(sf::Color::White);
		backGroundButton4_.setOutlineColor(sf::Color::White);

		Game::backGround = 1;
	}
	else if (backGroundButton3_.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		backGroundButton1_.setOutlineColor(sf::Color::White);
		backGroundButton2_.setOutlineColor(sf::Color::White);
		backGroundButton3_.setOutlineColor(sf::Color::Yellow);
		backGroundButton4_.setOutlineColor(sf::Color::White);

		Game::backGround = 2;
	}
	else if (backGroundButton4_.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		backGroundButton1_.setOutlineColor(sf::Color::White);
		backGroundButton2_.setOutlineColor(sf::Color::White);
		backGroundButton3_.setOutlineColor(sf::Color::White);
		backGroundButton4_.setOutlineColor(sf::Color::Yellow);

		Game::backGround = 3;
	}

	if(gridButton_.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && Game::isGrid == false)
	{
		gridButton_.setOutlineColor(sf::Color::Yellow);
		Game::isGrid = true;
	}
	else if(gridButton_.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && Game::isGrid == true)
	{
		gridButton_.setOutlineColor(sf::Color::Transparent);
		Game::isGrid = false;
	}
	
	
}


void GameOverScreen::handleInput(sf::RenderWindow& window)
{
	//goto gamescreen
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		Game::Screen = std::make_shared<GameScreen>();
	//close window
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		window.close();
	
	checkButtonPress(window);
}

void GameOverScreen::update(sf::Time delta)
{

}

void GameOverScreen::render(sf::RenderWindow& window)
{
	window.draw(backgroundSprite_);
	window.draw(text_);
	window.draw(backGroundButton1_);
	window.draw(backGroundButton2_);
	window.draw(backGroundButton3_);	
	window.draw(backGroundButton4_);
	window.draw(gridButton_);
	window.draw(gridText_);
}