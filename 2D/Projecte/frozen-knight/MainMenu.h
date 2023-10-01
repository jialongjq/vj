#ifndef _MAINMENU_INCLUDE
#define _MAINMENU_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "CollisionMap.h"
#include "BackgroundMap.h"
#include "Player.h"
#include "Door.h"
#include "Key.h"
#include "Skeleton.h"
#include "MenuUI.h"
#include <string>


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class MainMenu
{

public:
	MainMenu();
	~MainMenu();

	void reshape(int w, int h);

	void init();
	void update(int deltaTime);
	void render();
	void initShaders();
	void updateArrow(int action);
	int getStatus();
	int nextAction();
	bool inInstructions();
	bool inCredits();

private:
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	MenuUI* menuUI;
	int arrow;
	bool stageCleared;
	int clearedTime, clearedTimeElapsed;
	bool levelFinished;
	MediaPlayer* mediaPlayer;
};


#endif // _MainMenu_INCLUDE

