#ifndef _LEVELUI_INCLUDE
#define _LEVELUI_INCLUDE

#include "Sprite.h"

class LevelUI
{

public:
	void init(glm::vec2 InitialPos, ShaderProgram& shaderProgram);
	void render();
	void update(int deltaTime);
	void updateShield(bool shield);
	void updateHP(int hp);
	void updateStage(int stage);
	void updateTime(int time);
	void updateScore(int score);
	void setClear(bool clear);
	void updateGameStatus(int gameStatus);
	bool isReady();

private:
	Texture spritesheet;
	vector<Texture> spritesheets;
	vector<Sprite*> numbers, numbers2;
	vector<Sprite*> images;
	bool clear, ready, timeLimit;

	int readyTimeElapsed, gameStatus;
};

#endif 