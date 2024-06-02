#include <SFML/Graphics.hpp>

#include <memory>
#include <iostream>
#include <cmath>

#include "Snake.h"
#include "Game.h"
#include "Fruit.h"
#include "SnakeNode.h"
#include "GameOverScreen.h"

using namespace sfSnake;

const int Snake::InitialSize = 5;

Snake::Snake() : direction_{0, -SnakeNode::Height}, hitSelf_(false)
{
	initNodes();

	pickupBuffer_.loadFromFile("Sounds/ji.mp3");
	pickupSound_.setBuffer(pickupBuffer_);
	pickupSound_.setVolume(30);

	dieBuffer_.loadFromFile("Sounds/die.wav");
	dieSound_.setBuffer(dieBuffer_);
	dieSound_.setVolume(50);

	scores_ = 0;
}

void Snake::initNodes()
{
	for (int i = 0; i < Snake::InitialSize; ++i)
	{
		nodes_.push_back(SnakeNode(sf::Vector2f(
			Game::Width / 2 - SnakeNode::Width / 2,
			Game::Height / 2 - (SnakeNode::Height / 2) + (SnakeNode::Height * i))));
	}
}

void Snake::handleInput(sf::RenderWindow& window)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		direction_ = {0, -SnakeNode::Height};
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		direction_ = {0, SnakeNode::Height};
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		direction_ = {-SnakeNode::Width, 0};
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		direction_ = {SnakeNode::Width, 0};


	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)||
	   sf::Mouse::isButtonPressed(sf::Mouse::Right))
	   {
			static sf::Vector2i MousePosition;
			static sf::Vector2f HeadPosition;
			double directionSize;
			
			//get position
			MousePosition = sf::Mouse::getPosition(window);
			HeadPosition = nodes_[0].getPosition();
			//caculate vector
			sf::Vector2f directionVector{ MousePosition.x - HeadPosition.x,
										  MousePosition.y - HeadPosition.y};
			//caculate direction
			directionSize = std::sqrt(
							std::pow(directionVector.x,2) + 
							std::pow(directionVector.y,2));
			direction_.x = directionVector.x / directionSize * SnakeNode::Width;
			direction_.y = directionVector.y / directionSize * SnakeNode::Height;
	   }

	
}
//move and check collisions
void Snake::update(sf::Time delta)
{
	move();
	checkEdgeCollisions();
	checkSelfCollisions();
}

void Snake::checkFruitCollisions(std::vector<Fruit>& fruits)
{
	decltype(fruits.begin()) toRemove = fruits.end();

	for (auto it = fruits.begin(); it != fruits.end() ; ++it)
	{
		if (it->getBounds().intersects(nodes_[0].getBounds()))
			toRemove = it;
	}

	if (toRemove != fruits.end())
	{
		pickupSound_.play();
		const sf::Color & color = toRemove->getColor();
		if(color == sf::Color::Red){
			++scores_;
		}
		else if(color == sf::Color::Blue){
			scores_ += 2;
		}
		else if(color == sf::Color::Magenta){
			scores_ += 3;
		}
		//an egg.
		else if(color == sf::Color::Yellow){
			scores_ += 20;
		}
		grow();
		fruits.erase(toRemove);
	}
}

void Snake::grow()
{
	// switch (direction_)
	// {
	// case Direction::Up:
	// 	nodes_.push_back(SnakeNode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x,
	// 		nodes_[nodes_.size() - 1].getPosition().y + SnakeNode::Height)));
	// 	break;
	// case Direction::Down:
	// 	nodes_.push_back(SnakeNode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x,
	// 		nodes_[nodes_.size() - 1].getPosition().y - SnakeNode::Height)));
	// 	break;
	// case Direction::Left:
	// 	nodes_.push_back(SnakeNode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x + SnakeNode::Width,
	// 		nodes_[nodes_.size() - 1].getPosition().y)));
	// 	break;
	// case Direction::Right:
	// 	nodes_.push_back(SnakeNode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x - SnakeNode::Width,
	// 		nodes_[nodes_.size() - 1].getPosition().y)));
	// 	break;
	// }


	nodes_.push_back(SnakeNode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x + direction_.x,
	nodes_[nodes_.size() - 1].getPosition().y + direction_.y)));
}

unsigned Snake::getSize() const
{
	return nodes_.size();
}

bool Snake::hitSelf() const
{
	return hitSelf_;
}

void Snake::checkSelfCollisions()
{
	SnakeNode& headNode = nodes_[0];
	//ignore the collision of head node and the first body node
	for (decltype(nodes_.size()) i = 2; i < nodes_.size(); ++i)
	{
		if (headNode.getBounds().intersects(nodes_[i].getBounds()))
		{
			dieSound_.play();
			sf::sleep(sf::seconds(dieBuffer_.getDuration().asSeconds()));
			hitSelf_ = true;
		}
	}
}

void Snake::checkEdgeCollisions()
{
	SnakeNode& headNode = nodes_[0];

	if (headNode.getPosition().x <= 0)
		headNode.setPosition(Game::Width, headNode.getPosition().y);
	else if (headNode.getPosition().x >= Game::Width)
		headNode.setPosition(0, headNode.getPosition().y);
	else if (headNode.getPosition().y <= 0)
		headNode.setPosition(headNode.getPosition().x, Game::Height);
	else if (headNode.getPosition().y >= Game::Height)
		headNode.setPosition(headNode.getPosition().x, 0);
}

void Snake::move()
{
	for (decltype(nodes_.size()) i = nodes_.size() - 1; i > 0; --i)
	{
		nodes_[i].setPosition(nodes_.at(i - 1).getPosition());
	}

	nodes_[0].move(direction_.x,direction_.y);

	// switch (direction_)
	// {
	// case Direction::Up:
	// 	nodes_[0].move(0, -SnakeNode::Height);
	// 	break;
	// case Direction::Down:
	// 	nodes_[0].move(0, SnakeNode::Height);
	// 	break;
	// case Direction::Left:
	// 	nodes_[0].move(-SnakeNode::Width, 0);
	// 	break;
	// case Direction::Right:
	// 	nodes_[0].move(SnakeNode::Width, 0);
	// 	break;
	// }
}


int Snake::getScores() const
{
	return scores_;
}

void Snake::render(sf::RenderWindow& window)
{
	for (auto node : nodes_)
		node.render(window);
}