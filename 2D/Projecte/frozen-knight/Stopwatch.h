#ifndef _STOPWATCH_INCLUDE
#define _STOPWATCH_INCLUDE
#include "Collectable.h"
#include <random>

class Stopwatch : public Collectable
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
};

#endif

