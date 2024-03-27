#include <SFML/Graphics.hpp>
#include<vector>
#include<iostream>
class Enemy{


public :
	sf::Sprite shape;
	int Hp;
	int HpMax;
	Enemy(sf::Texture *texture,sf::Vector2u windowSize)
	{
		this->HpMax=rand()%3+1;
		this->Hp=HpMax;
		shape.setTexture(*texture);
		shape.setScale(0.2f,0.2f);
		shape.setPosition(windowSize.x-this->shape.getGlobalBounds().width,rand()%(int)(windowSize.y-this->shape.getGlobalBounds().height));
	}
	~Enemy()
	{

	}
};
class Bullet{
public:
  sf::Sprite shape;
  Bullet(sf::Texture *texture,sf::Vector2f vect)
  {
    this->shape.setTexture(*texture);
	this->shape.setScale(0.1f,0.1f);
	this->shape.setPosition(vect);
  }

};
class Player{

public:
	sf::Sprite shape;
	sf::Texture *texture;
	int Hp;
	int HpMax;
	std::vector<Bullet> bullets;
	Player(sf::Texture *texture)
	{
		this->HpMax=20;
		this->Hp=HpMax;
		this->texture=texture;
		this->shape.setTexture(*texture);
		this->shape.setScale(0.3f,0.3f);

	}
	~Player()
	{

	}
};

int main()
{
	sf::RenderWindow window(
		sf::VideoMode(640, 480),
		"Hello World");
	//sf::CircleShape shape(200);
	window.setFramerateLimit(60);

	/*init font*/
	sf::Font font;
	font.loadFromFile("../Font/Dosis-VariableFont_wght.ttf");

	/*init texture*/
	sf::Texture PlayerText;
	PlayerText.loadFromFile("../Images/spaceship.png");

	sf::Texture EnemyText;
	EnemyText.loadFromFile("../Images/Enemy.jpg");

	sf::Texture BulletText;
	BulletText.loadFromFile("../Images/Missile.jpg");

	//Player init 
	Player  player(&PlayerText);
	Bullet bullet(&BulletText,player.shape.getPosition());
	int shootTimer=10;

	//text init()
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(10.0f,10.0f);

	sf::Text GameOverText;
	GameOverText.setFont(font);
	GameOverText.setCharacterSize(30);
	GameOverText.setFillColor(sf::Color::Red);
	GameOverText.setPosition(100.f,window.getSize().y/2);
	GameOverText.setString("Game Over !");
	int score=0;

	sf::Text hpText;
	hpText.setFont(font);
	hpText.setCharacterSize(12);
	hpText.setFillColor(sf::Color::White);

	std::vector<Enemy> enemies;
	//enemies.emplace_back(Enemy(&EnemyText,window.getSize()));
	int enemySpawnTimer=0;

	sf::Text ehpText;
	ehpText.setFont(font);
	ehpText.setCharacterSize(12);
	ehpText.setFillColor(sf::Color::White);
	while(window.isOpen()) 
	{
		sf::Event event;
		while (
			window.pollEvent(event))
			if (event.type ==
			sf::Event::Closed)
				window.close();
		if(player.Hp>0)
		{
			//player moves
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				player.shape.move(0.f,-10.f);
			}
			//player move up on x axis
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				player.shape.move(-10.f,0.f);
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				player.shape.move(0.f,10.f);
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				player.shape.move(10.f,0.f);
			}


			//text allignment 

			hpText.setPosition(player.shape.getPosition().x,player.shape.getPosition().y-hpText.getGlobalBounds().height);
			hpText.setString(std::to_string(player.Hp)+"/"+std::to_string(player.HpMax));

			//collision with window 

			if(player.shape.getPosition().x<=0)//left
			{
				player.shape.setPosition(0.f,player.shape.getPosition().y);
			}
			if(player.shape.getPosition().x>window.getSize().x-player.shape.getGlobalBounds().width)//right of the screen
			{
				player.shape.setPosition(window.getSize().x-player.shape.getGlobalBounds().width,player.shape.getPosition().y);
			}
			if(player.shape.getPosition().y<=0)//top
			{
				player.shape.setPosition(player.shape.getPosition().x,0.0f);
			}
			if(player.shape.getPosition().y>window.getSize().y-player.shape.getGlobalBounds().height)//bottom of the screen
			{
				player.shape.setPosition(player.shape.getPosition().x,window.getSize().y-player.shape.getGlobalBounds().height);
			}

			//shooting
			//shoottimer 
			if(shootTimer<10)
			{
				shootTimer++;
			}
			//std::cout<<"shoot timer count is "<<shootTimer<<std::endl;
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && shootTimer>=0)
			{
				player.bullets.push_back(Bullet(&BulletText,player.shape.getPosition()));
				shootTimer=0;//reseting timer
			}
			for(size_t i=0;i<player.bullets.size();i++)
			{
				player.bullets[i].shape.move(20.f,0.f);
				if(player.bullets[i].shape.getPosition().x>window.getSize().x)
				{
					//std::cout<<"In eraser"<<std::endl;
					player.bullets.erase(player.bullets.begin()+i);
					break;
				}

				//enemy collision
				for(size_t k=0;k<enemies.size();k++)
				{
					if(player.bullets[i].shape.getGlobalBounds().intersects(enemies[k].shape.getGlobalBounds()))
					{
						if(enemies[k].Hp<=1)
						{
							score+=enemies[k].HpMax;
							enemies.erase(enemies.begin()+k);
						}
						else 
						{
							enemies[k].Hp--;
						}
						player.bullets.erase(player.bullets.begin()+i);
						break;

					}
				}
			}
			
			if(enemySpawnTimer<=20)
			{
				enemySpawnTimer++;
			}

			//enemy spawn
			if(enemySpawnTimer>20)
			{
				enemies.emplace_back(Enemy(&EnemyText,window.getSize()));
				enemySpawnTimer=0;
			}
			for(size_t i=0;i<enemies.size();i++)
			{
				enemies[i].shape.move(-5.0f,0.f);
				if(enemies[i].shape.getPosition().x<=0-enemies[i].shape.getGlobalBounds().width)
				{
					enemies.erase(enemies.begin()+i);
					break;
				}
				if(enemies[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
				{
					enemies.erase(enemies.begin()+i);
					player.Hp--;//player damage
					break;
				}
			}
		}
		scoreText.setString("Score : "+ std::to_string(score));
		//window.draw(shape);
		
		window.clear();

		//player
		window.draw(player.shape);

		//bullet
		for(size_t i=0;i<player.bullets.size();i++)
		{
			//std::cout<<"In player bullets"<<std::endl;
			window.draw(player.bullets[i].shape);
		}

		/*enemy*/
		for(size_t i=0;i<enemies.size();i++)
		{
			ehpText.setString(std::to_string(enemies[i].Hp)+"/"+std::to_string(enemies[i].HpMax));
			ehpText.setPosition(enemies[i].shape.getPosition().x,enemies[i].shape.getPosition().y-ehpText.getGlobalBounds().height);
			window.draw(ehpText);
			window.draw(enemies[i].shape);
		}
		
		//window.clear();
		//text ui
		window.draw(scoreText);
		window.draw(hpText);
		if(player.Hp<=0)
		{
			window.draw(GameOverText);
		}
		window.display();
	}
	return 0;
}

