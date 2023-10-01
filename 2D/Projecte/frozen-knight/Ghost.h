#ifndef _GHOST_INCLUDE
#define _GHOST_INCLUDE

#include "Enemy.h"
#include "Player.h"

class Ghost : public Enemy
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void setPlayer(Player* player);
	void update(int deltaTime);
	void updateOnCollision();
private:
	Player* player;
};

#endif