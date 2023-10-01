#ifndef _DOOR_INCLUDE
#define _DOOR_INCLUDE

#include "Entity.h"

class Door : public Entity
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void unlock();
	bool isUnlocked();

private:
};


#endif


