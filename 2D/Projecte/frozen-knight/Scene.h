#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "Game.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "CollisionMap.h"
#include "BackgroundMap.h"
#include "Player.h"
#include "Door.h"
#include "Key.h"
#include "Skeleton.h"
#include "Vampire.h"
#include "Ghost.h"
#include "LevelUI.h"
#include "Gem.h"
#include "Shield.h"
#include "Stopwatch.h"
#include <string>


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene
{

public:
	Scene();
	~Scene();

	void reshape(int w, int h);
	
	void init(int level, ShaderProgram& texProgram, int levelScore, int playerHP);
	void initMap();
	void initLevelUI();
	bool initPlayer(int hp);
	bool initDoor();

	bool initCollectables();
	bool initEnemies();
	void update(int deltaTime);
	void render();
	void changeInvulnerability();
	void showKey();

	int getPlayerHP();
	int getScore();

	void increaseScore(int amount);
	bool isFinished();
	bool isGameOver();

	void killPlayer(bool killedByTime);

	void changeHitboxVisibility();

private:
	bool collide(Entity &e1, Entity &e2);
	void collectKey();
	void enterDoor();

private:
	string fileName;
	TileMap *map;
	CollisionMap *collisionMap;
	BackgroundMap *backgroundMap;

	Player *player;
	bool firstSpawn = true;
	bool hasToRespawn = false;
	float invincibleTime;
	float accInvincibility;
	bool renderPlayer;
	bool invulnerable;
	float invulnerableTime;

	Door *door;
	Key *key;
	vector<Collectable*> collectables;
	int collectablesTimeElapsed = 0;
	vector<Enemy*> enemies;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	LevelUI* levelUI;
	int level;
	int score;
	int levelTime, levelTimeElapsed;
	bool stageCleared;
	bool gameOver;
	int gameOverTimeElapsed = 0;
	bool gameCompleted = false;
	int clearedTime, clearedTimeElapsed;
	bool clearedSoundPlayed = false;
	bool levelFinished;
	bool barrierActivated = false;
	int stopTimeElapsed;
	bool stop;
	int shakingTime;
	bool shake;

	MediaPlayer* mediaPlayer;
	bool gameOverSoundPlayed = false;
	bool gameCompletedSoundPlayed = false;
	bool readySoundPlayed = false;
	bool levelMusicPlaying = false;
};


#endif // _SCENE_INCLUDE

