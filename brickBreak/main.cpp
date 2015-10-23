#include <SFML/Graphics.hpp>
#include <iostream>
#include "Ball.h"
#include "Brique.h"
#include "Ball.h"
#include "Score.h"

sf::RectangleShape drawBrick(Brique brique)
{
	sf::RectangleShape brick(brique.getDim());
	brick.setPosition(brique.getPos());
	brick.setFillColor(brique.getColor());
	return brick;
}

sf::CircleShape drawBall(Ball ball)
{
	sf::CircleShape ballShape(ball.getRadius());
	ballShape.setPosition(ball.getPosition());
	ballShape.setFillColor(ball.getColor());
	return ballShape;
}

sf::RectangleShape drawBar(Bar bar)
{
	sf::RectangleShape barShape(bar.getDim());
	barShape.setPosition(bar.getPos());
	barShape.setFillColor(bar.getColor());
	return barShape;
}


int main()
{
	// Initialisation du score
	Score score(0, 0);
	score.initialize();
	
	/*Size of the space used to draw*/
	sf::Vector2f resolution(1920, 1080);
	/*Balls array*/
	std::vector<Ball> balls;
	/*Bricks array*/
	std::vector<Brique> bricks;
	/*default ball speed*/
	sf::Vector2f standardBallSpeed(5, 5);
	/*keyboard sensibility*/
	int keyboardSensibility(10);
	/*time between each graphical loop*/
	sf::Time loopTime = sf::microseconds(16666);

	Bar bar(sf::Vector2f((resolution.x - 350) / 2, resolution.y - 35), sf::Vector2f(350, 35), sf::Color::Red);
	sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Awesome brick breaker", sf::Style::Fullscreen);
	balls.push_back(Ball(sf::Vector2f(100, 100), 10, standardBallSpeed, sf::Color::Yellow));
	balls.push_back(Ball(sf::Vector2f(250, 150), 20, standardBallSpeed, sf::Color::Red));
	balls.push_back(Ball(sf::Vector2f(300, 100), 30, standardBallSpeed, sf::Color::Green));
	balls.push_back(Ball(sf::Vector2f(400, 100), 40, standardBallSpeed, sf::Color::Magenta));
	balls.push_back(Ball(sf::Vector2f(500, 100), 25, standardBallSpeed, sf::Color::Blue));
	balls.push_back(Ball(sf::Vector2f(600, 100), 15, standardBallSpeed, sf::Color::Cyan));
	bricks.push_back(Brique(sf::Vector2f(resolution.x / 2, resolution.y / 2), sf::Vector2f(200, 100), sf::Color::Yellow));
	bricks.push_back(Brique(sf::Vector2f(150, resolution.y / 2), sf::Vector2f(200, 100), sf::Color::Yellow));
	bricks.push_back(Brique(sf::Vector2f(300, 300), sf::Vector2f(200, 100), sf::Color::Yellow));
	bricks.push_back(Brique(sf::Vector2f(resolution.x / 2, 100), sf::Vector2f(200, 100), sf::Color::Yellow));
	bricks.push_back(Brique(sf::Vector2f(650, 300), sf::Vector2f(200, 100), sf::Color::Yellow));
	
	//force rendering to screen framerate
	window.setVerticalSyncEnabled(true);
	//hide the mouse
	window.setMouseCursorVisible(false);

	// program is running until window is closed
	while (window.isOpen())
	{
		//launch timer / needs to be at the beginning of the loop!!
		sf::Clock clock;

		// test null score
		if (score.getScore() == 0)
		{
			window.close();
		}

		// test to reduce score 
		if (balls.empty())
		{
			score.reduceScore();
			if (score.getScore() < 0)
			{
				window.close();
			}
			balls.push_back(Ball(sf::Vector2f(100, 100), 10, standardBallSpeed, sf::Color::Yellow));
			balls.push_back(Ball(sf::Vector2f(250, 150), 20, standardBallSpeed, sf::Color::Red));
			balls.push_back(Ball(sf::Vector2f(300, 100), 30, standardBallSpeed, sf::Color::Green));
			balls.push_back(Ball(sf::Vector2f(400, 100), 40, standardBallSpeed, sf::Color::Magenta));
			balls.push_back(Ball(sf::Vector2f(500, 100), 25, standardBallSpeed, sf::Color::Blue));
			balls.push_back(Ball(sf::Vector2f(600, 100), 15, standardBallSpeed, sf::Color::Cyan));
		}

		

		sf::Event event;
		while (window.pollEvent(event))
		{
			// enable window closing
			if (event.type == sf::Event::Closed || 
				(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();
			// capture mouse movement
			else if (event.type == sf::Event::MouseMoved)
			{
				//Convert window size to world size
				sf::Vector2f mouse=window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), window.getView());
				bar.setPosx(mouse.x - bar.getDim().x / 2);
			}
			else if (event.type == sf::Event::Resized)
			{
				sf::View view(sf::FloatRect(-(event.size.width - resolution.x) / 2, -(event.size.height - resolution.y) / 2
					, event.size.width, event.size.height));
				float zoom = std::max(resolution.x/ event.size.width, resolution.y/ event.size.height);
				view.zoom(zoom);
				window.setView(view);
			}
		}

		// keyboard direction action
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			bar.setPosx(bar.getPos().x - keyboardSensibility);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			bar.setPosx(bar.getPos().x + keyboardSensibility);
		}

		//check bar position
		bar.isInsideScreen(resolution);

		// color window in black
		window.clear(sf::Color::Black);
	

		// draw frame
		for (int i = 0; i < balls.size(); i++)
		{
			window.draw(drawBall(balls[i]));
			for (int j = 0; j < bricks.size(); j++)
			{
				window.draw(drawBrick(bricks[j]));
				balls[i].isColliding(bricks[j]);
			}
			for (int k = 1; i+k < balls.size(); k++)
			{
				balls[i].isColliding(balls[k+i]);
			}
			balls[i].isColliding(bar);
			balls[i].move(resolution);
			if (!balls[i].isInsideScreen(resolution))
			{
				balls.erase(balls.begin() + i);
			}
		}

		// draw bar
		window.draw(drawBar(bar));

		// show score
		window.draw(score.textScore(resolution));		
		
		// end of the draw frame
		window.display();
		sf::Time elapsed = clock.getElapsedTime();
		//framerate 60 hertz
		if (elapsed.asMicroseconds() < loopTime.asMicroseconds())
		{
			sf::sleep(loopTime - elapsed);
		}

	}

	return 0;
}