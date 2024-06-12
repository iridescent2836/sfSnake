#include <SFML/Graphics.hpp>

#include <memory>
#include <iostream>

#include "GameScreen.h"
#include "MenuScreen.h"
#include "Game.h"

using namespace sfSnake;

MenuScreen::MenuScreen()
{
	font_.loadFromFile("Fonts/game_over.ttf");

	text_.setFont(font_);
	text_.setString(
		"\n\n\n\n\n\n\nPress [SPACE] to play"
		"\n\nPress [ESC] to quit");
	text_.setFillColor(sf::Color::White);
	text_.setOutlineColor(sf::Color::Black);
	text_.setOutlineThickness(2);
	text_.setCharacterSize(Game::Width / 25);
	text_.setStyle(sf::Text::Bold);

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
}

void MenuScreen::handleInput(sf::RenderWindow& window)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		Game::Screen = std::make_shared<GameScreen>();
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		window.close();
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
}