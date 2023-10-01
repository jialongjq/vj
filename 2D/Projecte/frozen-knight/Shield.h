#ifndef _BARRIER_INCLUDE
#define _BARRIER_INCLUDE
#include "Collectable.h"
#include <random>

class Shield : public Collectable
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
};

#endif

