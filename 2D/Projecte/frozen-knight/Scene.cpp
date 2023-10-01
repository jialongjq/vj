#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include "Scene.h"
#include <typeinfo>

#define SCREEN_X 192
#define SCREEN_Y 96


Scene::Scene()
{
	map = NULL;
	player = NULL;
	collisionMap = NULL;
	backgroundMap = NULL;
	door = NULL;
	key = NULL;
	enemies.clear();
	levelUI = NULL;
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (collisionMap != NULL)
		delete collisionMap;
	if (backgroundMap != NULL)
		delete backgroundMap;
	if (door != NULL)
		delete door;
	if (key != NULL)
		delete key;
	if (enemies.size() != 0)
		enemies.clear();
	if (levelUI != NULL)
		delete levelUI;

}

void Scene::reshape(int w, int h) {
	float scale = h / 720.f;
	float offset = (w - 24 * 48 * scale) / 2;

	projection = glm::ortho(-offset, w - offset, float(h), 0.f);
	projection = glm::scale(projection, glm::vec3(scale));
}

void Scene::init(int level, ShaderProgram& shaderProgram, int levelScore, int playerHP)
{
	mediaPlayer = MediaPlayer::getInstance();
	mediaPlayer->stopAll();
	this->level = level;
	this->fileName = "levels/level0" + to_string(level) + ".txt";
	this->texProgram = shaderProgram;

	stageCleared = false;
	gameOver = false;

	initMap();

	initPlayer(playerHP);
	renderPlayer = true;
	invincibleTime = 4000;
	accInvincibility = 0;
	hasToRespawn = false;

	invulnerable = false;
	invulnerableTime = 0;

	initDoor();

	initCollectables();

	initEnemies();
	stop = false;
	stopTimeElapsed = 0;
	shakingTime = 0;
	shake = false;

	levelFinished = false;
	score = levelScore;
	levelTime = 60;
	clearedTimeElapsed = 0;
	levelTimeElapsed = 0;
	initLevelUI();

	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	levelUI->update(deltaTime);

	if (!readySoundPlayed) {
		mediaPlayer->playReadySound();
		readySoundPlayed = true;
	}

	if (levelUI->isReady()) {
		if (!levelMusicPlaying) {
			levelMusicPlaying = true;
			mediaPlayer->playLevelMusic(level);
		}
		if (firstSpawn) {
			firstSpawn = false;
			mediaPlayer->playSpawnSound();
		}
		if ((!gameOver && !stageCleared) || player->isSpawning() || (!stageCleared && gameOverTimeElapsed < 2000)) {
			// TIME
			if (!gameOver && !invulnerable && !stop) {
				levelTimeElapsed += deltaTime;
				if (levelTimeElapsed >= 1000) {
					levelTimeElapsed -= 1000;
					levelTime -= 1;
					if (levelTime >= 0 && levelTime <= 10) {
						mediaPlayer->playTickSound();
					}
					if (levelTime == 0) {
						killPlayer(true);
					}
					else if (levelTime == -1 && player->getHP() > 0) {
						levelTime = 60;
					}
					if (levelTime >= 0 && levelTime <= 60) {
						levelUI->updateTime(levelTime);
					}
				}
			}

			// INVINCIBILITY
			if (player->isInvincible()) {
				invincibleTime -= deltaTime;
				// Player blinks while invincible
				if (invincibleTime <= 2000 || barrierActivated) {
					accInvincibility += deltaTime;
					if (accInvincibility > 50) {
						accInvincibility -= 50;
						renderPlayer = !renderPlayer;
					}
				}
				if (player->isSpawning()) {
					renderPlayer = true;
				}
				// Player respawns after 2 seconds
				if (invincibleTime <= 2000 && hasToRespawn) {
					hasToRespawn = false;
					player->respawn();
					mediaPlayer->playSpawnSound();
				}
				else if (invincibleTime <= 2000 && player->getHP() == 0) {
					player->setPosition(glm::vec2(0, 0));
					gameOver = true;
					mediaPlayer->stopLevelMusic(level);
				}

				// Invincibility expires after 4 seconds
				if (invincibleTime <= 0) {
					player->setInvincible(false);
					invincibleTime = 4000;
					renderPlayer = true;
					barrierActivated = false;
				}
			}

			// INVULNERABILITY (BLINKING)
			if (invulnerable && !player->isInvincible()) {
				invulnerableTime += deltaTime;
				if (invulnerableTime >= 50) {
					invulnerableTime -= 50;
					renderPlayer = !renderPlayer;
				}
				if (player->isSpawning()) {
					renderPlayer = true;
				}
			}

			// STOPWATCH
			if (stop) {
				stopTimeElapsed += deltaTime;
				if (stopTimeElapsed >= 4000) {
					shakingTime += deltaTime;
					if (shakingTime >= 50) {
						shakingTime -= 50;
						shake = !shake;
						if (!shake) mediaPlayer->playShakingSound();
						for (unsigned int i = 0; i < enemies.size(); ++i) {
							if (shake)
								enemies[i]->setPosition(enemies[i]->getPosition() - glm::vec2(4, 0));
							if (!shake)
								enemies[i]->setPosition(enemies[i]->getPosition() + glm::vec2(4, 0));
						}
					}
				}
				if (stopTimeElapsed >= 5000) {
					stopTimeElapsed = 0;
					stop = false;
					if (shake) {
						shake = false;
						for (unsigned int i = 0; i < enemies.size(); ++i) {
							enemies[i]->setPosition(enemies[i]->getPosition() + glm::vec2(4, 0));
						}
					}
				}
			}

			// ENEMY INTERACTIONS
			for (unsigned int i = 0; i < enemies.size(); ++i) {
				if (!stop) {
					enemies[i]->update(deltaTime);
				}
				if (!invulnerable && !player->isInvincible() && !player->isSpawning() && !enemies[i]->isSpawning() && collide(*player, *enemies[i])) {
					if (!stop) {
						enemies[i]->updateOnCollision();
					}
					killPlayer(false);
				}
			}
			if (!gameOver) {
				player->update(deltaTime);
			}

			// COLLECTABLES
			// RANDOM SHOW CALL OF COLLECTABLE OBJECT WITH EACH SECOND PASSED
			collectablesTimeElapsed += deltaTime;
			if (collectablesTimeElapsed >= 1000) {
				collectablesTimeElapsed = 0;
				for (unsigned int i = 0; i < collectables.size(); ++i) {
					collectables[i]->show();
				}
			}

			key->update(deltaTime);

			for (unsigned int i = 0; i < collectables.size(); ++i) {
				collectables[i]->update(deltaTime);
				if (collectables[i]->isCollectable() && collide(*player, *collectables[i])) {
					collectables[i]->collect();
					string collectableType = typeid(*collectables[i]).name();
					if (collectableType == "class Gem") {
						score += collectables[i]->getValue();
						levelUI->updateScore(score);
					}
					else if (collectableType == "class Stopwatch") {
						stop = true;
					}
					else if (collectableType == "class Shield") {
						if (!player->hasShield()) {
							player->setShield(true);
							levelUI->updateShield(true);
						}
					}
				}
			}

			// KEY
			collectKey();

			// DOOR
			enterDoor();
		}
		else if (stageCleared) {
			// STAGE CLEARED
			clearedTimeElapsed += deltaTime;
			if (clearedTimeElapsed >= 500) {
				if (!clearedSoundPlayed) {
					levelUI->setClear(stageCleared);
					clearedSoundPlayed = true;
					mediaPlayer->playClearedSound();
				}
			}
			// ADD REMAINING TIME TO SCORE
			if (clearedTimeElapsed >= 1050) {
				if (levelTime > 0) {
					clearedTimeElapsed -= 50;
					levelTime -= 1;
					levelUI->updateTime(levelTime);
					increaseScore(10);
					mediaPlayer->playPointSound();
				}
			}
			// FINISH LEVEL
			if (clearedTimeElapsed >= 3000) {
				if (level == 3) {
					if (!gameCompletedSoundPlayed) {
						gameCompletedSoundPlayed = true;
						mediaPlayer->playHallelujahSound();
					}
					gameCompleted = true;
					levelUI->updateGameStatus(2);
					if (clearedTimeElapsed >= 5000) levelFinished = true;

				}
				else {
					levelFinished = true;
				}
			}
		}
	}
	if (gameOver) {
		gameOverTimeElapsed += deltaTime;
		if (gameOverTimeElapsed >= 2000) {
			if (!gameOverSoundPlayed) {
				gameOverSoundPlayed = true;
				mediaPlayer->playGameOverSound();
			}
			levelUI->updateGameStatus(1);
		}
		if (gameOverTimeElapsed >= 4000) {
			levelFinished = true;
		}
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	if (gameOverTimeElapsed < 2000 && !gameCompleted) {
		backgroundMap->render();
		map->render();
		door->render();
	}

	if (!gameCompleted) {
		if ((!gameOver && !stageCleared) || player->isSpawning() || (!stageCleared && gameOverTimeElapsed < 2000)) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(1, 1, 1, 0.5);
			for (unsigned int i = 0; i < enemies.size(); ++i) {
				enemies[i]->render();
			}
			glDisable(GL_BLEND);
			if (levelUI->isReady()) {
				key->render();
				for (unsigned int i = 0; i < collectables.size(); ++i) {
					collectables[i]->render();
				}
				if (!gameOver) {
					if (renderPlayer) {
						player->render();
					}
					else {
						player->renderHitbox();
					}
				}
			}
		}
	}

	levelUI->render();
}

bool Scene::collide(Entity& e1, Entity& e2) {
	glm::vec2 e1_hitboxOffset = e1.getHitboxOffset();
	float e1_x0 = e1.getPosition().x + e1_hitboxOffset.x;
	float e1_x1 = e1_x0 + e1.getSize().x;
	glm::vec2 e2_hitboxOffset = e2.getHitboxOffset();
	float e2_x0 = e2.getPosition().x + e2_hitboxOffset.x;
	float e2_x1 = e2_x0 + e2.getSize().x;
	if ((e1_x0 >= e2_x0 && e1_x0 <= e2_x1)
		|| (e1_x0 <= e2_x0 && e1_x1 >= e2_x0)
		|| (e1_x0 <= e2_x0 && e1_x1 >= e2_x1)
		|| (e1_x0 >= e2_x0 && e1_x1 <= e2_x1)) {
		float e1_y0 = e1.getPosition().y + e1_hitboxOffset.y;
		float e1_y1 = e1_y0 + e1.getSize().y;
		float e2_y0 = e2.getPosition().y + e2_hitboxOffset.y;
		float e2_y1 = e2_y0 + e2.getSize().y;
		if ((e1_y0 >= e2_y0 && e1_y0 <= e2_y1)
			|| (e1_y0 <= e2_y0 && e1_y1 >= e2_y0)
			|| (e1_y0 <= e2_y0 && e1_y1 >= e2_y1)
			|| (e1_y0 >= e2_y0 && e1_y1 <= e2_y1)) {
			return true;
		}
	}
	return false;
}

void Scene::collectKey() {
	if (key->isCollectable() && collide(*player, *key)) {
		key->collect();
	}
}

void Scene::enterDoor() {
	if (!stageCleared && door->isUnlocked() && collide(*player, *door) && player->isOnGround() && !player->isDead()) {
		player->despawn();
		stageCleared = true;
		mediaPlayer->stopLevelMusic(level);
		mediaPlayer->playSpawnSound();
	}
}

void Scene::showKey() {
	key->show();
}

void Scene::initMap() {
	map = TileMap::createTileMap(fileName, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map->setScene(this);
	backgroundMap = BackgroundMap::createBackgroundMap(fileName, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	collisionMap = CollisionMap::createCollisionMap(fileName);
	collisionMap->setScene(this);
}

void Scene::initLevelUI() {
	levelUI = new LevelUI();
	levelUI->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	levelUI->updateHP(player->getHP());
	levelUI->updateScore(score);
	levelUI->updateStage(level);
	levelUI->updateTime(levelTime);
}

bool Scene::initPlayer(int hp) {
	ifstream fin;
	string line;
	stringstream sstream;
	fin.open(fileName.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	while (line.compare(0, 6, "PLAYER") != 0) {
		getline(fin, line);
	}
	if (line.compare(0, 6, "PLAYER") != 0) {
		return false;
	}

	float pos_x, pos_y;
	getline(fin, line);
	sstream.str(line);
	sstream >> pos_x >> pos_y;

	fin.close();

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(pos_x * map->getTileSize(), pos_y * map->getTileSize()));
	player->setSpawnPosition(glm::vec2(pos_x * map->getTileSize(), pos_y * map->getTileSize()));
	player->setSize(glm::vec2(24, 48));
	player->setTileMap(map);
	player->setCollisionMap(collisionMap);
	player->setHP(hp);

	return true;
}

bool Scene::initDoor() {
	ifstream fin;
	string line;
	stringstream sstream;
	fin.open(fileName.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	while (line.compare(0, 4, "DOOR") != 0) {
		getline(fin, line);
	}
	if (line.compare(0, 4, "DOOR") != 0) {
		return false;
	}

	float pos_x, pos_y;
	getline(fin, line);
	sstream.str(line);
	sstream >> pos_x >> pos_y;

	fin.close();
	door = new Door();
	door->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	door->setPosition(glm::vec2(pos_x * map->getTileSize(), pos_y * map->getTileSize()));
	door->setSize(glm::vec2(32, 48));
	return true;
}

bool Scene::initCollectables() {
	ifstream fin;
	string line;
	stringstream sstream;
	fin.open(fileName.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	while (line.compare(0, 3, "END") != 0) {
		bool isGem = line.compare(0, 3, "GEM") == 0;
		bool isShield = line.compare(0, 6, "SHIELD") == 0;
		bool isStopwatch = line.compare(0, 9, "STOPWATCH") == 0;
		bool isKey = line.compare(0, 3, "KEY") == 0;
		if (isGem || isShield || isStopwatch || isKey) {
			getline(fin, line);
			sstream.str(line);
			float pos_x, pos_y;
			sstream >> pos_x >> pos_y;
			if (isGem) {
				Gem* gem = new Gem();
				gem->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
				gem->setPosition(glm::vec2(pos_x * map->getTileSize(), pos_y * map->getTileSize()));
				gem->setSpawnPosition(glm::vec2(pos_x * map->getTileSize(), pos_y * map->getTileSize()));
				gem->setSize(glm::vec2(20, 28));
				collectables.push_back(gem);
			}
			else if (isShield) {
				Shield* shield = new Shield();
				shield->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
				shield->setPosition(glm::vec2(pos_x * map->getTileSize(), pos_y * map->getTileSize()));
				shield->setSpawnPosition(glm::vec2(pos_x * map->getTileSize(), pos_y * map->getTileSize()));
				shield->setSize(glm::vec2(28, 36));
				collectables.push_back(shield);
			}
			else if (isStopwatch) {
				Stopwatch* stopwatch = new Stopwatch();
				stopwatch->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
				stopwatch->setPosition(glm::vec2(pos_x * map->getTileSize(), pos_y * map->getTileSize()));
				stopwatch->setSpawnPosition(glm::vec2(pos_x * map->getTileSize(), pos_y * map->getTileSize()));
				stopwatch->setSize(glm::vec2(28, 32));
				collectables.push_back(stopwatch);
			}
			else if (isKey) {
				key = new Key();
				key->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
				key->setPosition(glm::vec2(pos_x * map->getTileSize(), pos_y * map->getTileSize()));
				key->setSpawnPosition(glm::vec2(pos_x * map->getTileSize(), pos_y * map->getTileSize()));
				key->setSize(glm::vec2(16, 28));
				key->setDoor(door);
				map->setKey(key);
			}
		}
		getline(fin, line);
	}
	fin.close();
	return true;
}
bool Scene::initEnemies() {
	ifstream fin;
	string line;
	stringstream sstream;
	fin.open(fileName.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	while (line.compare(0, 3, "END") != 0) {
		bool isSkeleton = line.compare(0, 8, "SKELETON") == 0;
		bool isVampire = line.compare(0, 7, "VAMPIRE") == 0;
		bool isGhost = line.compare(0, 7, "GHOST") == 0;

		if (isSkeleton || isVampire || isGhost) {
			int n_enemies;
			getline(fin, line);
			sstream.str(line);
			sstream >> n_enemies;
			for (int i = 0; i < n_enemies; ++i) {
				float pos_x, pos_y;
				char orientation;
				getline(fin, line);
				sstream.str(line);
				sstream >> pos_x >> pos_y >> orientation;
				if (isSkeleton) {
					Skeleton* skeleton = new Skeleton();
					skeleton->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
					skeleton->setPosition(glm::vec2(pos_x * map->getTileSize(), pos_y * map->getTileSize()));
					skeleton->setOrientation(orientation);
					skeleton->setSize(glm::vec2(24, 48));
					skeleton->setCollisionMap(collisionMap);
					enemies.push_back(skeleton);
				}
				else if (isVampire) {
					Vampire* vampire = new Vampire();
					vampire->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
					vampire->setPosition(glm::vec2(pos_x * map->getTileSize(), pos_y * map->getTileSize()));
					vampire->setOrientation(orientation);
					vampire->setSize(glm::vec2(24, 48));
					vampire->setCollisionMap(collisionMap);
					enemies.push_back(vampire);
				}
				else {
					Ghost* ghost = new Ghost();
					ghost->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
					ghost->setPlayer(player);
					ghost->setPosition(glm::vec2(pos_x * map->getTileSize(), pos_y * map->getTileSize()));
					ghost->setOrientation(orientation);
					ghost->setSize(glm::vec2(24, 48));
					ghost->setCollisionMap(collisionMap);
					enemies.push_back(ghost);
				}
			}
		}
		getline(fin, line);
	}
	fin.close();
	return true;
}
void Scene::changeInvulnerability() {
	invulnerable = !invulnerable;
	if (invulnerable == false)
		renderPlayer = true;
}

void Scene::increaseScore(int amount) {
	score += amount;
	levelUI->updateScore(score);
}

bool Scene::isFinished() {
	return levelFinished;
}

bool Scene::isGameOver() {
	return gameOver;
}

int Scene::getPlayerHP() {
	return player->getHP();
}

int Scene::getScore() {
	return score;
}

void Scene::killPlayer(bool killedByTime) {
	if (player->getHP() > 0) {
		if (!killedByTime && !invulnerable && !player->isInvincible() && !player->isSpawning() && !stageCleared) {
			if (player->hasShield()) {
				mediaPlayer->playShieldSound();
				barrierActivated = true;
				player->setShield(false);
				levelUI->updateShield(false);
				player->setInvincible(true);
			}
			else {
				mediaPlayer->playFrozenSound();
				player->die(false);
				player->setInvincible(true);
				invincibleTime = 4000;
				if (player->getHP() > 0) {
					hasToRespawn = true;
				}
			}
		}
		else if (killedByTime) {
			mediaPlayer->playFrozenSound();
			player->die(true);
			player->setInvincible(true);
			invincibleTime = 4000;
			if (player->getHP() > 0) {
				hasToRespawn = true;
			}
		}
	}
	if (player->getHP() == 0) {
		hasToRespawn = false;
	}
	levelUI->updateHP(player->getHP());
}
void Scene::changeHitboxVisibility() {
	player->changeHitboxVisibility();
	glm::vec2 hitboxOffset = player->getHitboxOffset();
	key->changeHitboxVisibility();
	door->changeHitboxVisibility();
	for (unsigned int i = 0; i < enemies.size(); i++) {
		enemies[i]->changeHitboxVisibility();
	}
	for (unsigned int i = 0; i < collectables.size(); i++) {
		collectables[i]->changeHitboxVisibility();
	}
}
