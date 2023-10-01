#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

#include "Entity.h"
#include "CollisionMap.h"
#include "MediaPlayer.h"

class Enemy : public Entity
{
public:
	virtual void updateOnCollision() = 0;
	void setCollisionMap(CollisionMap* collisionMap);
	void setOrientation(char orientation);
	void changeOrientation();
protected:
	CollisionMap* collisionMap;
};

#endif

