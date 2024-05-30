#include <SFML/Graphics.hpp>

#include "Fruit.h"

using namespace sfSnake;

const float Fruit::Radius = 5.f;

Fruit::Fruit(sf::Vector2f position,int color)
{
	shape_.setPosition(position);
	shape_.setRadius(Fruit::Radius);
	switch(color){
		case 0: 
			shape_.setFillColor(sf::Color::Red);
			break;
		case 1:
			shape_.setFillColor(sf::Color::Blue);
			break;
		case 2:
			shape_.setFillColor(sf::Color::Magenta);
			break;
		default:
			shape_.setFillColor(sf::Color::Red);
			break;
	}
	
}

void Fruit::render(sf::RenderWindow& window)
{
	window.draw(shape_);
}

void Fruit::setColor(const sf::Color color){
	this->shape_.setFillColor(color);
}


sf::FloatRect Fruit::getBounds() const
{
	return shape_.getGlobalBounds();
}