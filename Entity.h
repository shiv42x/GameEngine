#pragma once
#include "Components.h"
#include <memory>
#include <string>

class Entity
{
	friend class EntityManager;
	const size_t      m_id    = 0;
	const std::string m_tag   = "default";
	bool              m_alive = true;

	Entity(const std::string& tag, const size_t id);

public:

	// component pointers
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CShape>     cShape;
	std::shared_ptr<CCollision> cCollision;
	std::shared_ptr<CInput>     cInput;
	std::shared_ptr<CScore>     cScore;
	std::shared_ptr<CLifespan>  cLifespan;

	// private member access functions
	bool               isAlive()  const;
	const std::string& tag()      const;
	const size_t       id()       const;
	void               destroy();
};