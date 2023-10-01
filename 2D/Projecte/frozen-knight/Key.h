#ifndef _KEY_INCLUDE
#define _KEY_INCLUDE

#include "Sprite.h"
#include "Collectable.h"
#include "Door.h"

class Key : public Collectable
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void setDoor(Door *door);
	void show();
	void collect();
private:
	Door* door;
};


#endif


