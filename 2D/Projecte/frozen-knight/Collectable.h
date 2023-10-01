#ifndef _COLLECTABLE_INCLUDE
#define _COLLECTABLE_INCLUDE
#include "Entity.h"
class Collectable : public Entity
{
public:
	void update(int deltaTime);
	void render();
	void collect();
	void show();
	bool isCollectable();
	virtual int getValue();
protected:
	static const int HIDDEN = 0;
	static const int SHOWN = 1;
	static const int COLLECTED = 2;
	int status;
	int shownTimeElapsed = 0;
	int blinkingTimeElapsed = 0;
	bool renderCollectable = true;
};
#endif


