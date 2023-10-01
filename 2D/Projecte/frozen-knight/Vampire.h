#ifndef _VAMPIRE_INCLUDE
#define _VAMPIRE_INCLUDE
#include "Enemy.h"
#include "CollisionMap.h"
class Vampire : public Enemy
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void updateOnCollision();

private:
	glm::vec2 vampireHitboxOffset = glm::vec2(12, 0);
	glm::vec2 vampireSize = glm::vec2(24, 48);
	glm::vec2 batHitboxOffset = glm::vec2(6, 12);
	glm::vec2 batSize = glm::vec2(36, 24);
	bool bat = false;
	int transformationTimeElapsed = 0;
	int direction = 0;
};
#endif

