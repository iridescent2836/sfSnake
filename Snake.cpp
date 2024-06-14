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
Snake::Snake() : direction_{0, -40.0f * 0.8}, radius_(20.0f) ,hitSelf_(false)
{

	pickupBuffer_.loadFromFile("Sounds/ji.mp3");
	pickupSound_.setBuffer(pickupBuffer_);
	pickupSound_.setVolume(30);

	dieBuffer_.loadFromFile("Sounds/die.wav");
	dieSound_.setBuffer(dieBuffer_);
	dieSound_.setVolume(50);

	//load texture
	headTexture_.loadFromFile("texture/head1.png");
	bodyTexture_.loadFromFile("texture/body.png");



	//set scarf
	scarfTexture_.loadFromFile("texture/scarf.png");
	scarfSize_ = {radius_/2.0f, radius_*1.8f};

	scarf_.setTexture(scarfTexture_);
	// scarf_.setScale(scarfSize_.x, scarfSize_.y);
	scarf_.setScale(radius_/scarfTexture_.getSize().x/2.4f, radius_*1.1/scarfTexture_.getSize().y*2.0f);
	scarf_.setOrigin(scarf_.getTextureRect().width/2.0f, scarf_.getTextureRect().height/2.0f);

	scarfHead_.setTexture(scarfTexture_);
	// scarfHead_.setScale(scarfSize_.y, scarfSize_.x);
	scarfHead_.setScale(radius_/scarfTexture_.getSize().x*1.8f, radius_/scarfTexture_.getSize().y/2.0f);
	scarfHead_.setOrigin(scarfHead_.getTextureRect().width/2.0f, scarfHead_.getTextureRect().height/2.0f);





	initNodes();


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
		nodes_[i].setOrigin(radius_, radius_);
		
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


	nodes_.push_back(sf::CircleShape{radius_});
	sf::Vector2f posOfLastNode = nodes_[nodes_.size() - 2].getPosition();
	sf::Vector2f PosOfLastLastNode = nodes_[nodes_.size() - 3].getPosition();
	// float direction = nodes_[nodes_.size() - 2].getRotation() * 3.14159265358979323846 / 180 + 3.14159265358979323846 / 2;
	// int distance = (radius_ + radius_) * 0.8;
	// nodes_[nodes_.size() - 1].setPosition(posOfLastNode.x - distance*std::cos(direction),posOfLastNode.y-distance*std::sin(direction));
	nodes_[nodes_.size() - 1].setPosition(posOfLastNode.x,posOfLastNode.y);
	nodes_[nodes_.size() - 1].setOrigin(radius_, radius_);
	nodes_[nodes_.size() - 1].setRotation(nodes_[nodes_.size() - 2].getRotation());
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
		if (dis(headNode.getPosition(),nodes_[i].getPosition()) < radius_ * 1.5)
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
		nodes_[i].setRotation(nodes_.at(i - 1).getRotation());
	}

	nodes_[0].move(direction_.x,direction_.y);
	nodes_[0].setRotation(std::atan2(direction_.y,direction_.x)  * 180 / 3.14159265358979323846 + 90);

	//让scarfHead在head的头的带方向的正下方
	float degree = nodes_[0].getRotation()/180.0f*3.14159265358979323846 + 3.14159265358979323846/2;
	scarfHead_.setPosition(nodes_[0].getPosition().x+(radius_ - scarfSize_.x/3.2f)*std::cos(degree),
						   nodes_[0].getPosition().y+(radius_ - scarfSize_.x/3.2f)*std::sin(degree));

	scarfHead_.setRotation(nodes_[0].getRotation());

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
		// window.draw(test);
	for (auto node : nodes_)
		window.draw(node);
	//draw scarf
	window.draw(scarfHead_);

	for(int i = 1; i < nodes_.size(); ++i){
		//让scarfHead在head的头的带方向的正右方
		float degree = nodes_[i].getRotation()/180.0f*3.14159265358979323846 ;
		scarf_.setPosition(nodes_[i].getPosition().x+(radius_*0.5 )*std::cos(degree),
							nodes_[i].getPosition().y+(radius_*0.5 )*std::sin(degree));

		scarf_.setRotation(nodes_[i].getRotation());
		window.draw(scarf_);
	}

}