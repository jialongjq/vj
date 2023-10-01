#ifndef _GEM_INCLUDE
#define _GEM_INCLUDE
#include "Collectable.h"
#include <random>

class Gem : public Collectable
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	int getValue();
private:
	int color;
	int value;
};

#endif

