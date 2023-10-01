#ifndef _MENUUI_INCLUDE
#define _MENUUI_INCLUDE

#include "Sprite.h"

class MenuUI
{

public:
	void init(glm::vec2 InitialPos, ShaderProgram& shaderProgram);
	void render();
	void update(int deltaTime);
	void updateStatus(int arrow);
	void nextAction(int status);
	bool inInstructions();
	bool inCredits();

private:
	vector<Texture> spritesheets;
	vector<Sprite*> numbers;
	vector<Sprite*> images;
	glm::vec2 pos, arrowPos;
	bool instructions, credits, ready;

	int readyTimeElapsed;
};

#endif 