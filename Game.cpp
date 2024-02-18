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

	m_entities.update();

	sEnemySpawner();
	sMovement();
	sCollision();
	sUserInput();
	sRender();

	// increment current frame
	m_currentFrame++;
}

void Game::setPaused(bool paused)
{

}

// respawn player in middle of screen
void Game::spawnPlayer()
{
	// TODO: finish adding all properties of player with correct values

	// We create every entity by calling EntityManager.addEntity(tag)
	// This returns a std::shared_ptr<Entity>, so we use auto to save typing
	auto entity = m_entities.addEntity("player");


	// Give entity Transform so it spawns at 200, 200 with velo 1, 1, angle 0
	entity->cTransform = std::make_shared<CTransform>(
		Vec2(200.0f, 200.0f),
		Vec2(1.0f, 1.0f), 0.0f);

	// Give entity shape 
	entity->cShape = std::make_shared<CShape>(
		32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);

	entity->cInput = std::make_shared<CInput>();

	m_player = entity;
}

// spawn enemy at random position
void Game::spawnEnemy()
{
	// TODO: make sure enemy is spawned properly with m_enemyConfig vars
	// enemy must be spawned completely within the window

	// record last time an enemy spawned
	m_lastEnemySpawnTime = m_currentFrame; 
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	// TODO: spawn small enemies at location of input enemy e
	// when create smaller enemy, have to read values of orig enemy
	// - spawn a number of small enemies equal to vertices of original
	// - set each small enemy to same color as original
	// - small enemies are worth double points of original enemy
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos)
{
	// TODO: implement the spawning of a bullet which travels to target
	// bullet speed is given as scalar speed
	// must set velocity by using formula
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	// TODO: implement special weapon
}

void Game::sMovement()
{
	// TODO: implement all entity movement in this function
	// should read the m_player->cInput component to determine if the player is moving

	// Sample
	m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
	m_player->cTransform->pos.y += m_player->cTransform->velocity.y;

}

void Game::sLifeSpan()
{
	// for all entities:
	// if entity has no lifespan component, skip it
	// if entity has > 0 remaining lifespan, subtract 1
	// if it has lifespan and is alive,
	//		scale alpha channel properly
	// if it has lifespan and its time is up, 
	//		destroy entity
}

void Game::sCollision()
{
	// TODO: implement all proper collisions between entities
	// be sure to use collision radius, not shape radius
}

void Game::sEnemySpawner()
{
	// TODO: code which implements enemy spaning
	// use m_currentFrame - m_lastEnemySpawnTime to determine
	// how long it has been since last enemy spawned
}

void Game::sRender()
{
	// TODO: change code to draw ALL entities
	// currently draws player entity

	m_window.clear();

	// set pos of shape based on entity's transform->pos
	m_player->cShape->circle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);

	// set rotation of shape based on entity's transform angle
	m_player->cTransform->angle += 1.0f;
	m_player->cShape->circle.setRotation(m_player->cTransform->angle);

	// draw entity's sf::CircleShape
	m_window.draw(m_player->cShape->circle);

	m_window.display();
}

void Game::sUserInput()
{
	// TODO: handle user input
	// note only set player's input component vars here
	// dont implement players movement logic here
	// movement system will read vars set here

	sf::Event event;
	while (m_window.pollEvent(event))
	{
		// when window is closed
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		// when key pressed
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				std::cout << "W pressed\n";
				// TODO: set player's input component "up" to true
				break;
			
			// TODO: add remaining case statements
			default: break;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				std::cout << "W released\n";
				// TODO: set player's input component "up" to false
				break;

				// TODO: add remaining case statements
			default: break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				std::cout << "Left mouse clicked at " << event.mouseButton.x << ", " << event.mouseButton.y << '\n';
				// call spawnBullet
			}

			if (event.mouseButton.button == sf::Mouse::Right)
			{
				std::cout << "Right mouse clicked at " << event.mouseButton.x << ", " << event.mouseButton.y << '\n';
				// call spawnSpecialWeapon
			}
		}
	}
}