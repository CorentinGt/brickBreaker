#ifndef DEF_BRICK
#define DEF_BRICK

#include "Ball.h"
#include "BallsHandler.h"
#include <SFML/Graphics.hpp>


class Brick {
public:
	Brick(sf::Vector2f, sf::Vector2f, sf::Color);
	virtual void draw(sf::RenderWindow&);
	sf::Vector2f getDim() const;
	sf::Vector2f getPos() const;
	sf::Color getColor() const;
	int onCollision();
	double getTanBrique() const;
	virtual bool isDestroyed(std::vector<Ball>&);

protected:
	sf::Vector2f pos;
	sf::Vector2f dim;
	sf::Color color;
	double tanBrick;
	std::string texture;
	int hits;
	bool destroyed;

};






#endif