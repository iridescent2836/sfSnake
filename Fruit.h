#ifndef FRUIT_H
#define FRUIT_H

#include <SFML/Graphics.hpp>

namespace sfSnake
{
class Fruit
{
public:
	Fruit(sf::Vector2f position = sf::Vector2f(0, 0),int color = 0);

	void render(sf::RenderWindow& window);

	void setColor(const sf::Color& color);
	const sf::Color & getColor() const;
	sf::FloatRect getBounds() const;
	const sf::Vector2f & getPosition() const;
	float getRadius() const;

private:
	sf::CircleShape shape_;

	static const float Radius;
};
}

#endif