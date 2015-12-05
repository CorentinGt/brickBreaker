#ifndef DEF_BRICK
#define DEF_BRICK

#include "Ball.h"
#include "Particle.h"
#include "BallsHandler.h"
#include <SFML/Graphics.hpp>


class Brick {
public:
	Brick(sf::Vector2f, sf::Vector2f, sf::Texture*&);
	void draw(sf::RenderWindow&);
	sf::Vector2f getDim() const;
	sf::Vector2f getPos() const;
	sf::Texture* getTexture() const;
	void onCollision();
	float getTanBrick() const;
	virtual bool isDestroyed(std::vector<Ball>&, std::vector<class Particle>&);
	int getHits();

protected:
	sf::Vector2f pos;
	sf::Vector2f dim;
	sf::Texture* tex;
	int hits=1;

};



#endif