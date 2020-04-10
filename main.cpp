#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>
#include <iostream>
#include <cmath>

#define DEBUG true

const float PLAYER_Y = 270.f;
const float BALL_X = 285.f;
const float BALL_Y = 300.f;
const int HIT_THRESHOLD = 1;

const float BOUNDRY_TOP = 0.f;
const float BOUNDRY_BOT = 585.f;
const float BOUNDRY_LEFT = 0;
const float BOUNDRY_RIGHT = 585;
const float BALL_SPEED = 2.5f;
const float PLAYER_SPEED = 15.f;

void renderPause(sf::RenderWindow &window, sf::Font &font);
void renderGame(sf::RenderWindow &window, sf::Font &font, float states[3][2], int score[2], int dirX, float ballAngle);
void renderEndGame(sf::RenderWindow &window, sf::Font &font, int winner);
int main()
{
	int state = 0;
	int score[2] = { 0,0 };
	int dirX = 1;
	float ballAngle = 3.f;

	sf::RenderWindow window(sf::VideoMode(600, 600), "Pong");
	sf::Font font;
	font.loadFromFile("resources\\fonts\\munro.ttf");

	float playerStates[3][2] = { {10.f,PLAYER_Y},{570.f,PLAYER_Y},{BALL_X,BALL_X} };
	while(window.isOpen())
	{
		if(state == 0)
		{
			renderPause(window, font);
		}
		else if(state == 1)
		{
			std::cout << "entering game" << std::endl;
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			{
				return 0;
			}
			if(state == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				state = 1;
			}else if(state == 1)
			{
				renderGame(window, font, playerStates, score, dirX, ballAngle);
				state = 0;
			}
		}
	}
}

void renderPause(sf::RenderWindow &window, sf::Font &font)
{
	sf::Text titleText;

		
	titleText.setFont(font);
	titleText.setFillColor(sf::Color::White);
	titleText.setCharacterSize(40);
	titleText.setPosition(240.f, 50.f);
	titleText.setString("PONG");
	sf::Text instr = titleText;
	instr.setString("Press space to play.");
	instr.setPosition(210.f, 100.f);
	instr.setCharacterSize(20);

	window.clear();
	window.draw(titleText);
	window.draw(instr);
	window.display();
}
void renderGame(sf::RenderWindow& window, sf::Font& font, float states[3][2], int score[2],int dirX, float ballAngle)
{
	const float pi = 3.14f;
	bool inGame = true;
	int dirY = 1;
	int winner = 0;

	sf::RectangleShape p1;
	sf::RectangleShape p2;
	sf::CircleShape ball;

	p1.setFillColor(sf::Color::White);
	p1.setSize(sf::Vector2f(20, 100));

	p2.setFillColor(sf::Color::White);
	p2.setSize(sf::Vector2f(20, 100));

	ball.setFillColor(sf::Color::White);
	ball.setRadius(15.f);
	ball.setPosition(states[2][0],states[2][1]);

	sf::Clock clock;

	while(window.isOpen() && inGame == true)
	{
		sf::Event event;
		// update the ball
		sf::Time time = clock.getElapsedTime();
		int noclip = 0;
			p2.setPosition(states[1][0], states[1][1]);

		while(window.pollEvent(event))
		{
			// handle the controls
			if(event.type == sf::Event::Closed)
			{
				inGame = false;
				break;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				inGame = false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				if(states[0][1] >= BOUNDRY_TOP)
				{
					states[0][1] -= PLAYER_SPEED;
				}
			}
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				if (states[0][1] <= BOUNDRY_BOT)
				{
					states[0][1] += PLAYER_SPEED;
				}
			}

			// set player positions
			p1.setPosition(states[0][0], states[0][1]);

		}
		p2.setPosition(p2.getPosition().x,ball.getPosition().y);
		 // render the game screen
		if(time.asMilliseconds() % 15 == 0)
		{
			clock.restart();
			// ball movement

			// boundry checks
			sf::Vector2f ballPos = ball.getPosition();
			if(ballPos.y >= BOUNDRY_BOT)
			{
				ballAngle = -ballAngle;
			}else if(ballPos.y <= BOUNDRY_TOP)
			{
				ballAngle = -ballAngle;
			}
			// score checks
			if(ballPos.x > BOUNDRY_RIGHT)
			{
				winner = 1;
				inGame = false;
			}else if(ballPos.x < BOUNDRY_LEFT)
			{
				winner = 2;
				inGame = false;
			}
			// paddle checks
			if(noclip > 0) noclip--;
			if(ball.getPosition().x + ball.getRadius() >= p1.getPosition().x - HIT_THRESHOLD
				&& ball.getPosition().x - ball.getRadius() <= p1.getPosition().x + HIT_THRESHOLD
				&& ball.getPosition().y >= p1.getPosition().y - p1.getSize().y /2
				&& ball.getPosition().y <= p1.getPosition().y + p1.getSize().y /2
				&& noclip == 0)
			{
				ballAngle = -ballAngle + (std::rand() % 5);
			}
			if(ball.getPosition().x + ball.getRadius() >= p2.getPosition().x - HIT_THRESHOLD
				&& ball.getPosition().x - ball.getRadius() <= p2.getPosition().x + HIT_THRESHOLD
				&& ball.getPosition().y >= p2.getPosition().y - p2.getSize().y /2
				&& ball.getPosition().y <= p2.getPosition().y + p2.getSize().y /2
				&& noclip ==0)
			{
				ballAngle = -ballAngle + (std::rand() % 5);
			}
			std::cout << p1.getPosition().x << "   " << ball.getPosition().x << std::endl;
			ball.move(std::cos(ballAngle) * BALL_SPEED, std::sin(ballAngle) * BALL_SPEED);
			window.clear();
			window.draw(p1);
			window.draw(p2);
			window.draw(ball);
			window.display();
		}
	}
	renderEndGame(window, font, winner);
}
void renderEndGame(sf::RenderWindow &window, sf::Font &font, int winner)
{
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(40);
	text.setPosition(240.f, 50.f);
	switch(winner)
	{
		case 1:
		text.setString("Player 1 Wins!");
		break;
		case 2:
		text.setString("Player 2 Wins!");
		break;
		default:
		text.setString("Everybody loses!");
	}
	bool endGame = true;
	while(window.isOpen() && endGame == true)
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				endGame = false;
			}
		}
		window.clear();
		window.draw(text);
		window.display();
	}
}
