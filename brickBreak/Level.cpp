#include "Level.h"

//class Brick//
Level::Level(sf::Vector2f resolution, int number, Score& scoring)
{

	/*keyboard sensibility*/
	keyboardSensibility=20;
	//creation of the ball
	bar=Bar(sf::Vector2f((resolution.x - 350) / 2, resolution.y - 35), sf::Vector2f(350, 25), sf::Color::Red,false);
	bar.addBall();
	createBricks(number);
	score = scoring;
	levelFinished = false;
}

void Level::forward(sf::Vector2f resolution, sf::RenderWindow& window, Menu& menu)
{
	// test null score
	if (score.getScore() == 0)
	{
		sf::Font* font = new sf::Font;
		if (!(*font).loadFromFile("arial.ttf"))
		{
			std::puts("error loading font\n");
		}
		sf::Text text("Game Over", *font);
		text.setCharacterSize(100);
		text.setStyle(sf::Text::Bold);
		text.setColor(sf::Color::Red);
		sf::FloatRect rect = text.getGlobalBounds();
		text.setPosition(sf::Vector2f(window.getSize().x/2 - rect.width/2, window.getSize().y/2 - rect.height/2));
		sf::Time t1 = sf::seconds(2);
		window.clear();
		window.draw(text);
		window.display();
		sf::sleep(t1);
		finishLevel();
	}

	// test null bricks
	if (isOver())
	{
		sf::Font* font = new sf::Font;
		if (!(*font).loadFromFile("arial.ttf"))
		{
			std::puts("error loading font\n");
		}
		sf::Text text("You won!", *font);
		text.setCharacterSize(100);
		text.setStyle(sf::Text::Bold);
		text.setColor(sf::Color::Red);
		sf::FloatRect rect = text.getGlobalBounds();
		text.setPosition(sf::Vector2f(window.getSize().x / 2 - rect.width / 2, window.getSize().y / 2 - rect.height / 2));
		sf::Time t1 = sf::seconds(2);
		window.clear();
		window.draw(text);
		window.display();
		sf::sleep(t1);
		finishLevel();
	}

	// keyboard direction action
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		setBarPosition(getBarPosition() - keyboardSensibility);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		setBarPosition(getBarPosition() + keyboardSensibility);
	}
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space)|| sf::Mouse::isButtonPressed(sf::Mouse::Left)) && bar.isBall())
	{
		// launch ball in bar
		balls.launchBall(bar);
	}

	// test to reduce score 
	if (balls.aliveBalls() == 0 && !bar.isBall())
	{
		score.reduceScore();
		if (score.getScore() < 0)
		{
			window.close();
		}
		bar.addBall();
	}
	// implement the V bounce
	int loop(0);
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::V)) && (loop == 0))
	{
		if (getBarPositionY() <= resolution.y -15) setBarPositionY(getBarPositionY() + 15);
		posArrayBarAdd(1);
		loop = 1;
	}
	else if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::V))) 
	{
		setBarPositionY(resolution.y - 35);
		posArrayBarAdd(0);
		loop = 0;
	}
	//check bar position
	bar.isInsideScreen(resolution);

	balls.move(resolution, bar, myBricks);

	deleteDestroyedBricks();
	drawComponents(window, resolution);

}

void Level::setBarPosition(float barpos)
{
	bar.setPosx(barpos);
}

void Level::setBarPositionY(float barpos)
{
	bar.setPosy(barpos);
}

float Level::getBarPosition()
{
	return bar.getPos().x;
}

float Level::getBarPositionY()
{
	return bar.getPos().y;
}

void Level::createBricks(int level)
{

	float cpt_x = 0, cpt_y = 0;
	//creation of the bricks depending on the level selected
	if (level == 0)
	{
		for (cpt_y = 0; cpt_y < 5; cpt_y++) {
			for (cpt_x = 0; cpt_x < 6; cpt_x++) {
				myBricks.push_back(new Brick(sf::Vector2f(300 + 2 * cpt_x * 120, 200 + cpt_y * 60), sf::Vector2f(100, 40), sf::Color::Yellow));
				if (cpt_x < 5) {
					if (cpt_y == 0 || cpt_y == 2 || cpt_y == 4) {
						myBricks.push_back(new BallBrick(sf::Vector2f(300 + (2 * cpt_x + 1) * 120, 200 + cpt_y * 60), sf::Vector2f(100, 40), sf::Color::Yellow));
					}
					else {
						myBricks.push_back(new StrongBrick(sf::Vector2f(300 + (2 * cpt_x + 1) * 120, 200 + cpt_y * 60), sf::Vector2f(100, 40), sf::Color::Yellow, 3));

					}
				}
			}
		}
	}
	else if (level==1)
	{
		for (cpt_y = 0; cpt_y < 5; cpt_y++) {
			for (cpt_x = 0; cpt_x < 6; cpt_x++) {
				myBricks.push_back(new Brick(sf::Vector2f(300 + 2 * cpt_x * 120, 200 + cpt_y * 60), sf::Vector2f(100, 40), sf::Color::Yellow));
				if (cpt_x < 5) {
					if (cpt_y == 0 || cpt_y == 2 || cpt_y == 4) {
						myBricks.push_back(new StrongBrick(sf::Vector2f(300 + (2 * cpt_x + 1) * 120, 200 + cpt_y * 60), sf::Vector2f(100, 40), sf::Color::Transparent, -1));
					}
					else {
						myBricks.push_back(new BallBrick(sf::Vector2f(300 + (2 * cpt_x + 1) * 120, 200 + cpt_y * 60), sf::Vector2f(100, 40), sf::Color::Yellow));
					}
				}
			}
		}
	}
	else
	{
		for (cpt_y = 0; cpt_y < 5; cpt_y++) {
			for (cpt_x = 0; cpt_x < 6; cpt_x++) {
				if (cpt_y == 0 || cpt_y == 2 || cpt_y == 4)
				{
					myBricks.push_back(new Brick(sf::Vector2f(300 + 2 * cpt_x * 120, 200 + cpt_y * 60), sf::Vector2f(100, 40), sf::Color::Yellow));
				}
				else
				{
					myBricks.push_back(new StrongBrick(sf::Vector2f(300 + 2 * cpt_x * 120, 200 + cpt_y * 60), sf::Vector2f(100, 40), sf::Color::Yellow, 3));
				}
				if (cpt_x < 5) {
					if (cpt_y == 0 || cpt_y == 2 || cpt_y == 4)
					{
						myBricks.push_back(new StrongBrick(sf::Vector2f(300 + (2 * cpt_x + 1) * 120, 200 + cpt_y * 60), sf::Vector2f(100, 40), sf::Color::Transparent, -1));
					}
					else if (cpt_x == 0 || cpt_x == 2 || cpt_x == 4)
					{
						myBricks.push_back(new BallBrick(sf::Vector2f(300 + (2 * cpt_x + 1) * 120, 200 + cpt_y * 60), sf::Vector2f(100, 40), sf::Color::Yellow));
					}
					else
					{
						myBricks.push_back(new Brick(sf::Vector2f(300 + (2 * cpt_x + 1) * 120, 200 + cpt_y * 60), sf::Vector2f(100, 40), sf::Color::Yellow));
					}
				}
			}
		}
	}
}

void Level::deleteDestroyedBricks()
{
	for (unsigned int j = 0; j < myBricks.size(); j++)
	{
		//detection of destroyed bricks
		if (myBricks[j]->isDestroyed(balls.getBalls()))
		{
			myBricks.erase(myBricks.begin() + j);
		}
	}
}

void Level::drawComponents(sf::RenderWindow& window, sf::Vector2f& resolution)
{

	// show score
	score.draw(resolution, window);
	bar.draw(window);
	balls.draw(window);
	for (unsigned int j = 0; j < myBricks.size(); j++)
	{
		myBricks[j]->draw(window);
	}
}

bool Level::isOver()
{
	unsigned int nbToDestroy=0;
	for (unsigned int i = 0; i < myBricks.size(); i++)
	{
		if (myBricks[i]->getHits() > 0)
		{
			nbToDestroy++;
		}
	}
	return nbToDestroy == 0;
}

void Level::finishLevel()
{
	levelFinished = true;
}

bool Level::isFinished()
{
	return levelFinished;
}
void Level::posArrayBarAdd(float posy)
{
	bar.posArrayPushBack(posy);
}
