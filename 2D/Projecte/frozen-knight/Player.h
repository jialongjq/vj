#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Entity.h"
#include "TileMap.h"
#include "CollisionMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player : public Entity
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void setTileMap(TileMap *tileMap);
	void setCollisionMap(CollisionMap* collisionMap);
	void die(bool unconditional);
	bool isDead();
	bool isJumping();
	bool isInvincible();
	bool isOnGround();
	bool hasShield();
	void setShield(bool shield);
	void setInvincible(bool invincible);
	void respawn();
	void despawn();
	int getHP();
	void setHP(int hp);
	void renderHitbox();
private:
	bool bJumping;
	int jumpHeight, startY;
	int jumpingTimeElapsed = 0;
	int hp;
	bool invincible;
	bool shield;
	bool falling = false;
	TileMap *map;
	CollisionMap* collisionMap;
};


#endif // _PLAYER_INCLUDE


