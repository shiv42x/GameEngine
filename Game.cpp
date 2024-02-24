#include "Game.h"
#include <iostream>
#include <fstream>


Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& path)
{
	// TODO: read in config here
	// use premade PlayerConfig, EnemyConfig, BulletConfig vars
	std::ifstream fin(path);

	// fin >> m_playerConfig.SR >> m_playerConfig.CR

	m_window.create(sf::VideoMode(1280, 720), "ECS Geometry Wars");
	m_window.setFramerateLimit(60);
	spawnPlayer();
}

void Game::run()
{
	// TODO: add pause functionality
	// some systems should function while paused (rendering)
	// some systems shouldn't
	while (m_window.isOpen() && m_running)
	{
		m_entities.update();

		sEnemySpawner();
		sMovement();
		sCollision();
		sUserInput();
		sLifeSpan();

		sRender();


		// increment current frame
		m_currentFrame++;
	}
}

void Game::setPaused(bool paused)
{
	m_paused = true;
}

// respawn player in middle of screen
void Game::spawnPlayer()
{
	// TODO: finish adding all properties of player with correct values
	// this returns a std::shared_ptr<Entity>, so we use auto to save typing
	auto entity = m_entities.addEntity("player");

	entity->cTransform = std::make_shared<CTransform>(
		Vec2(1280/2, 720/2),
		Vec2(0.0f, 0.0f), 0.0f);

	// Give player entity shape 
	entity->cShape = std::make_shared<CShape>(
		32.0f, 8, sf::Color(10, 10, 10), sf::Color(0, 255, 0), 4.0f);
	entity->cInput = std::make_shared<CInput>();
	m_player = entity;
}

// spawn enemy at random position
void Game::spawnEnemy()
{
	// TODO: make sure enemy is spawned properly with m_enemyConfig vars
	// enemy must be spawned completely within the window
	auto enemy = m_entities.addEntity("enemy");

	// generate enemy properties
	// TODO: seed 
	// range syntax => (max - min + 1) + min
	int radius = rand() % 11 + 25;

	// ensure spawns completely within window
	int x = rand() % (1281 - radius) + radius;
	int y = rand() % (721 - radius) + radius;

	int sides = rand() % 6 + 3;
	int x_velo = (rand() % 6) - 2;
	int y_velo = (rand() % 6) - 2;

	// if enemy spawns too close to player (within 50px), roll position again
	if ((m_player->cTransform->pos.x - x < 50) && m_player->cTransform->pos.y - y < 50)
	{
		x = rand() % (1281 - radius) + radius;
		y = rand() % (721 - radius) + radius;
	}

	enemy->cTransform = std::make_shared<CTransform>(
		Vec2(x, y),
		Vec2(x_velo, y_velo),
		0.0f
	);

	enemy->cShape = std::make_shared<CShape>(
		radius,
		sides,
		sf::Color(150, 0, 0, 255),
		sf::Color(175, 175, 175, 255),
		4.0f
	);

	//enemy->cLifespan = std::make_shared<CLifespan>(360);

	// record last time an enemy spawned
	m_lastEnemySpawnTime = m_currentFrame; 
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> spawningEntity)
{
	// TODO: spawn small enemies at location of input enemy e
	// when create smaller enemy, have to read values of orig enemy
	// - spawn a number of small enemies equal to vertices of original
	// - set each small enemy to same color as original
	// - small enemies are worth double points of original enemy
}

void Game::spawnBullet(std::shared_ptr<Entity> spawningEntity, const Vec2& mousePos)
{
	float bulletSpeed = 10.0f;
	Vec2 velocityVec = (mousePos - spawningEntity->cTransform->pos);
	velocityVec.normalize();
	velocityVec *= bulletSpeed;

	auto bullet = m_entities.addEntity("bullet");
	
	bullet->cTransform = std::make_shared<CTransform>(
		spawningEntity->cTransform->pos,
		velocityVec, 
		0.0f
	);

	bullet->cShape = std::make_shared<CShape>(
		10.0f, 50, sf::Color(125, 125, 125, 255),
		sf::Color(255, 255, 255, 255), 2
	);

	// lifespan of 60 = 1 sec because 60fps framerate limit
	bullet->cLifespan = std::make_shared<CLifespan>(60);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> spawningEntity)
{
	// TODO: implement special weapon
}

void Game::sMovement()
{
	// Player
	Vec2 playerVelocity(0.0, 0.0);

	if (m_player->cInput->up)
	{
		playerVelocity.y -= 1.0f;
	}
	if (m_player->cInput->left)
	{
		playerVelocity.x -= 1.0f;
	}
	if (m_player->cInput->down)
	{
		playerVelocity.y += 1.0f;
	}
	if (m_player->cInput->right)
	{
		playerVelocity.x += 1.0f;
	}

	// scale movespeed, then add velocity vector to pos
	playerVelocity *= m_player->cTransform->moveSpeedMulti;
	m_player->cTransform->pos += playerVelocity;


	// move entities
	for (auto& e : m_entities.getEntities()) 
	{	
		// dont update player entities
		if (e->tag() != "player")
		{
			e->cTransform->pos += e->cTransform->velocity;
		}
	}
}

void Game::sLifeSpan()
{
	for (auto& e : m_entities.getEntities())
	{
		if (e->cLifespan)
		{
			if (e->cLifespan->remaining > 0)
			{
				float alphaMultiplier = float(e->cLifespan->remaining) / float(e->cLifespan->total);		
				sf::Color fillColor = e->cShape->circle.getFillColor();
				sf::Color outlineColor = e->cShape->circle.getOutlineColor();
				e->cShape->circle.setFillColor(sf::Color
				(
					fillColor.r,
					fillColor.g,
					fillColor.b,
					(255 * alphaMultiplier)
				)
				);
				e->cShape->circle.setOutlineColor(sf::Color
				(
					outlineColor.r,
					outlineColor.g,
					outlineColor.b,
					(255 * alphaMultiplier)
				)
				);
				e->cLifespan->remaining--;
			}
			else
			{
				e->destroy();
			}
		}
	}
}

void Game::sCollision()
{
	// TODO: implement all proper collisions between entities
	// be sure to use collision radius, not shape radius
	for (auto& e : m_entities.getEntities())
	{
		if (e->tag() != "player") 
		{
			// if going out of bounds, reverse respectve component to simulate 'bounce'
			if (((e->cTransform->pos.x - e->cShape->circle.getRadius()) <= 0) || ((e->cTransform->pos.x + e->cShape->circle.getRadius()) >= 1280))
			{
				e->cTransform->velocity.x = -(e->cTransform->velocity.x);
			}

			if (((e->cTransform->pos.y - e->cShape->circle.getRadius()) <= 0) || ((e->cTransform->pos.y + e->cShape->circle.getRadius()) >= 720))
			{
				e->cTransform->velocity.y = -(e->cTransform->velocity.y);
			}
		}

		// keep player within bounds
		// better way to do this?
		if (e->tag() == "player")
		{
			if ((e->cTransform->pos.x - e->cShape->circle.getRadius()) <= 0)
			{
				e->cTransform->pos.x = e->cShape->circle.getRadius();
			}
			if ((e->cTransform->pos.x + e->cShape->circle.getRadius()) >= 1280)
			{
				e->cTransform->pos.x = (1280 - e->cShape->circle.getRadius());
			}
			if ((e->cTransform->pos.y - e->cShape->circle.getRadius()) <= 0)
			{
				e->cTransform->pos.y = e->cShape->circle.getRadius();
			}
			if ((e->cTransform->pos.y + e->cShape->circle.getRadius()) >= 720)
			{
				e->cTransform->pos.y = (720 - e->cShape->circle.getRadius());
			}

		}

	}
}

void Game::sEnemySpawner()
{
	// spawn enemy every 1.5 seconds
	if (m_currentFrame - m_lastEnemySpawnTime > 90)
	{
		spawnEnemy();
	}
}

void Game::sRender()
{
	m_window.clear();

	// set pos of shape based on entity's transform->pos
	m_player->cShape->circle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);

	// set rotation of shape based on entity's transform angle
	m_player->cTransform->angle += 1.0f;
	m_player->cShape->circle.setRotation(m_player->cTransform->angle);

	// draw entity's sf::CircleShape
	m_window.draw(m_player->cShape->circle);

	for (auto e : m_entities.getEntities())
	{
		e->cShape->circle.setPosition(
			e->cTransform->pos.x,
			e->cTransform->pos.y
			);
		e->cShape->circle.setRotation(
			e->cTransform->angle += 1.0f
		);
		m_window.draw(e->cShape->circle);
	}
	m_window.display();
}

void Game::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_player->cInput->up = true;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = true;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = true;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = true;
				break;
			default: break;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_player->cInput->up = false;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = false;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = false;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = false;
				break;
			default: break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
			}

			if (event.mouseButton.button == sf::Mouse::Right)
			{
				std::cout << "Right mouse clicked at " << event.mouseButton.x << ", " << event.mouseButton.y << '\n';
				// call spawnSpecialWeapon
			}
		}
	}
}