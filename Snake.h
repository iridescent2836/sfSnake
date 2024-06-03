#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>

#include "SnakeNode.h"
#include "Fruit.h"

namespace sfSnake
{
	enum class Direction
	{
		Left, Right, Up, Down
	};

class Snake
{
public:
	Snake();

	void handleInput(sf::RenderWindow& window);
	void update(sf::Time delta);
	void render(sf::RenderWindow& window);

	void checkFruitCollisions(std::vector<Fruit>& fruits);

	bool hitSelf() const;

	int getScores() const;

	unsigned getSize() const;

private:
	void move();
	void grow();
	void checkEdgeCollisions();
	void checkSelfCollisions();
	void initNodes();

	void setTexture(sf::CircleShape& node, bool isHead);

	bool hitSelf_;

	sf::Vector2f position_;
	sf::Vector2f direction_;

	sf::SoundBuffer pickupBuffer_;
	sf::Sound pickupSound_;

	sf::SoundBuffer dieBuffer_;
	sf::Sound dieSound_;

	//std::vector<SnakeNode> nodes_;
	std::vector<sf::CircleShape> nodes_;


	sf::Texture headTexture_;
	sf::Texture bodyTexture_;

	float radius_;
	

	int scores_;

	static const int InitialSize;
};
}

#endif