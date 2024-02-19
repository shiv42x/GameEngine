#pragma once
#include "Entity.h"
#include "EntityManager.h"
#include <SFML/Graphics.hpp>

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L, SB; float S; };

class Game
{
private:
	sf::RenderWindow	m_window;						// The window we'll use to draw graphics
	EntityManager		m_entities;						// The vector of entities
	sf::Font			m_font;							// The font we'll use for the text
	sf::Text			m_scoreText;					// the Score's text
	PlayerConfig		m_playerConfig;					// the player's properties
	EnemyConfig			m_enemyConfig;					// the enemy's properties
	BulletConfig		m_bulletConfig;					// the bullet's properties
	int					m_score = 0;
	int					m_currentFrame = 0;
	int					m_lastEnemySpawnTime = 0;
	bool				m_paused = false;				// If game logic is updated
	bool				m_running = true; 				// If the game is running

	std::shared_ptr<Entity> m_player;

	void init(const std::string& config);			// initialize the gamestate with a config file path
	void setPaused(bool paused);					// Pause the game

	void sMovement();								// Systems: entity movement
	void sUserInput();								// Systems: User Input
	void sLifeSpan();								// Systems: lifespan of the entities
	void sRender();									// Systems: Render
	void sEnemySpawner();							// Systems: Spawns enemies
	void sCollision();								// Systems: Collision detection

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> spawningEntity);
	void spawnBullet(std::shared_ptr<Entity> spawningEntity, const Vec2& mousePos);
	void spawnSpecialWeapon(std::shared_ptr<Entity> spawningEntity);

public:
	Game(const std::string& configFile);
	void run();
};