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


//Here we directly use -20.0f instead of 2*radius_ because using radius_ during construction will cause a bug.
//*0.8 is to ensure that the nodes partially overlap with each other; otherwise, it would look too unnatural.
// this *0.8 also occurs in the handleinput.
Snake::Snake() : direction_{0, -20.0f * 0.8}, radius_(10.0f) ,hitSelf_(false)
{
	initNodes();

	pickupBuffer_.loadFromFile("Sounds/ji.mp3");
	pickupSound_.setBuffer(pickupBuffer_);
	pickupSound_.setVolume(30);

	dieBuffer_.loadFromFile("Sounds/die.wav");
	dieSound_.setBuffer(dieBuffer_);
	dieSound_.setVolume(50);

	//load texture
	headTexture_.loadFromFile("texture/test1.jpg");
	bodyTexture_.loadFromFile("texture/test2.png");


	scores_ = 0;
}

void Snake::setTexture(sf::CircleShape& node, bool isHead){
	if(isHead){
		node.setTexture(&headTexture_);
	}
	else{
		node.setTexture(&bodyTexture_);
	}


}

void Snake::initNodes()
{
	for (int i = 0; i < Snake::InitialSize; ++i)
	{
		nodes_.push_back(sf::CircleShape {radius_});
		nodes_[i].setPosition(Game::Width / 2.0f - radius_ ,
							  Game::Height / 2.0f - radius_ + radius_* 2 * i);
		
		setTexture(nodes_[i], !i);
	}



	// //set test in the position of head
	// test_.setPosition(nodes_[0].getPosition());
}

void Snake::handleInput(sf::RenderWindow& window)
{

	//*0.8 is to ensure that the nodes partially overlap with each other; otherwise, it would look too unnatural.
	//this *0.8 also occurs in the constuctor.
	float distance = (radius_+radius_) * 0.8;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		direction_ = {0, -distance};
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		direction_ = {0, distance};
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		direction_ = {-distance, 0};
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		direction_ = {distance, 0};


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
			direction_.x = directionVector.x / directionSize * distance;
			direction_.y = directionVector.y / directionSize * distance;
	   }

	
}
//move and check collisions
void Snake::update(sf::Time delta)
{
	move();
	checkEdgeCollisions();
	checkSelfCollisions();
}

inline float dis(const sf::Vector2f & node1,const sf::Vector2f & node2){
	return std::sqrt(std::pow(static_cast<double>(node1.x - node2.x),2) + 
					 std::pow(static_cast<double>(node1.y - node2.y),2));
}


void Snake::checkFruitCollisions(std::vector<Fruit>& fruits)
{
	decltype(fruits.begin()) toRemove = fruits.end();


	for (auto it = fruits.begin(); it != fruits.end() ; ++it)
	{
		//this cost too much resources 
		// if (dis(it->getPosition(),nodes_[0].getPosition()) < it->getRadius() + radius_)
		// 	toRemove = it;

		//cost less resource and less accurate but more easy.
		if(it->getBounds().intersects(nodes_[0].getGlobalBounds())){
			toRemove = it;
		}
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
			scores_ += 50;
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


/*
	nodes_.push_back(SnakeNode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x + direction_.x,
	nodes_[nodes_.size() - 1].getPosition().y + direction_.y)));

*/

	nodes_.push_back(sf::CircleShape{radius_});
	sf::Vector2f posOfLastNode = nodes_[nodes_.size() - 2].getPosition();
	nodes_[nodes_.size() - 1].setPosition(posOfLastNode.x + direction_.x,posOfLastNode.y+direction_.y);
	setTexture(nodes_[nodes_.size() - 1], 0);
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
	sf::CircleShape& headNode = nodes_[0];
	//ignore the collision of head node and the first body node
	for (decltype(nodes_.size()) i = 2; i < nodes_.size(); ++i)
	{
		// cost less resource but less accurate
		// if(headNode.getGlobalBounds().intersects(nodes_[i].getGlobalBounds()))

		//cost more resource but more accurate
		if (dis(headNode.getPosition(),nodes_[i].getPosition()) < radius_ + radius_)
		{
			dieSound_.play();
			sf::sleep(sf::seconds(dieBuffer_.getDuration().asSeconds()));
			hitSelf_ = true;
		}
	}
}

void Snake::checkEdgeCollisions()
{
	sf::CircleShape& headNode = nodes_[0];

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
		window.draw(node);
	// window.draw(test_);
}