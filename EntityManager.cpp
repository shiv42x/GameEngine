#include "EntityManager.h"

EntityManager::EntityManager()
	: m_totalEntities(0)
{}

void EntityManager::update()
{
	// TODO: add entities from m_entitiesToAdd
	// - add them to vector of all entities
	// - add them to vector inside the map with tag as key

	// remove dead entities from vector of all entities
	removeDeadEntities(m_entities);

	// remove dead entities from each vector in the entity map
	for (auto& [tag, entityVec] : m_entityMap)
	{
		removeDeadEntities(entityVec);
	}
}

void EntityManager::removeDeadEntities(EntityVec& Vec)
{
	// TODO: remove all dead entities from the input vector
	// this is called by the update() function
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(tag, m_totalEntities++));
	m_entitiesToAdd.push_back(entity);
	return entity;
}

const EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
	// TODO: incorrect, return correct vector from map
	// return map indexed by tag
	return m_entities;
}

const std::map<std::string, EntityVec>& EntityManager::getEntityMap()
{
	return m_entityMap;
}