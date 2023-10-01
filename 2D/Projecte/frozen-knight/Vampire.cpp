#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Vampire.h"

#define FALL_STEP 4

enum VampireAnims
{
	MOVE_RIGHT, MOVE_LEFT, BAT_RIGHT, BAT_LEFT
};

enum Direction
{
	RIGHT_UP, RIGHT_DOWN, LEFT_UP, LEFT_DOWN
};

void Vampire::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	initHitbox(tileMapPos, shaderProgram, "vampire");
	hitboxOffset = vampireHitboxOffset;
	hitboxSprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(0.5, 1.0), &hitboxSpritesheet, &shaderProgram);
	hitboxSprite->setNumberAnimations(2);

	hitboxSprite->setAnimationSpeed(0, 0);
	hitboxSprite->addKeyframe(0, glm::vec2(0.f, 0.0f));

	hitboxSprite->setAnimationSpeed(1, 0);
	hitboxSprite->addKeyframe(1, glm::vec2(0.5f, 0.0f));

	hitboxSprite->changeAnimation(0);

	initSpawn(tileMapPos, shaderProgram);
	spritesheet.loadFromFile("images/vampire.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(0.25f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE_RIGHT, 4);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(BAT_RIGHT, 4);
	sprite->addKeyframe(BAT_RIGHT, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(BAT_RIGHT, glm::vec2(0.5f, 0.25f));
	sprite->addKeyframe(BAT_RIGHT, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(BAT_RIGHT, glm::vec2(0.5f, 0.75f));

	sprite->setAnimationSpeed(MOVE_LEFT, 4);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25, 0.5f));

	sprite->setAnimationSpeed(BAT_LEFT, 4);
	sprite->addKeyframe(BAT_LEFT, glm::vec2(0.75, 0.f));
	sprite->addKeyframe(BAT_LEFT, glm::vec2(0.75, 0.25f));
	sprite->addKeyframe(BAT_LEFT, glm::vec2(0.75, 0.5f));
	sprite->addKeyframe(BAT_LEFT, glm::vec2(0.75, 0.75f));

	sprite->changeAnimation(MOVE_RIGHT);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void Vampire::update(int deltaTime)
{
	if (spawning) {
		spawnSprite->update(deltaTime);
		spawningTimeElapsed += deltaTime;

		if (spawningTimeElapsed >= spawningTime) {
			spawning = false;
			spawningTimeElapsed = 0;
		}
		return;
	}

	sprite->update(deltaTime);
	
	if (!bat) {
		// VAMPIRE COLLISION
		int shift = 0;
		if (sprite->animation() == MOVE_RIGHT)
			shift = size.x;
		if (sprite->animation() == MOVE_LEFT)
			shift = -size.x;
		bool collisionDown = collisionMap->enemyCollisionDown(position + glm::ivec2(shift, 2), size, hitboxOffset);
		bool collisionLeft = (sprite->animation() == MOVE_LEFT) && collisionMap->enemyCollisionLeft(position + glm::ivec2(-2, 0), size, hitboxOffset);
		bool collisionRight = (sprite->animation() == MOVE_RIGHT) && collisionMap->enemyCollisionRight(position + glm::ivec2(2, 0), size, hitboxOffset);
		if ((!collisionDown) || collisionLeft || collisionRight) {
			if (sprite->animation() == MOVE_RIGHT) {
				sprite->changeAnimation(MOVE_LEFT);
			}
			else {
				sprite->changeAnimation(MOVE_RIGHT);
			}
		}
		// MOVEMENT
		if (sprite->animation() == MOVE_RIGHT) {
			position.x += 2;
		}
		else {
			position.x -= 2;
		}
		// TRANSFORMATION
		transformationTimeElapsed += deltaTime;
		if (transformationTimeElapsed >= 5000) {
			if (!collisionMap->enemyCollisionLeft(position, batSize, batHitboxOffset) &&
				!collisionMap->enemyCollisionRight(position, batSize, batHitboxOffset)) {
				MediaPlayer* mediaPlayer = MediaPlayer::getInstance();
				mediaPlayer->playEnemySound("transformation");
				size = batSize;
				hitboxOffset = batHitboxOffset;
				transformationTimeElapsed = 0;
				bat = true;
				spawning = true;
				spawnSprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
				hitboxSprite->changeAnimation(1);
				if (sprite->animation() == MOVE_LEFT) {
					direction = LEFT_UP;
					sprite->changeAnimation(BAT_LEFT);
				}
				else {
					direction = RIGHT_UP;
					sprite->changeAnimation(BAT_RIGHT);
				}
			}
		}
	}
	else {
		// BAT COLLISION
		if (collisionMap->isBetweenCollisionsY(position, size, hitboxOffset)) {
			if (collisionMap->enemyCollisionLeft(position + glm::ivec2(-2, 0), size, hitboxOffset)) {
				sprite->changeAnimation(BAT_RIGHT);
				if (direction == LEFT_UP || direction == LEFT_DOWN) direction = RIGHT_UP;
			}
			else if (collisionMap->enemyCollisionRight(position + glm::ivec2(2, 0), size, hitboxOffset)) {
				sprite->changeAnimation(BAT_LEFT);
				if (direction == RIGHT_UP || direction == RIGHT_DOWN) direction = LEFT_UP;
			}
			if (direction == LEFT_UP || direction == LEFT_DOWN) {
				position.x -= 2;
			}
			else {
				position.x += 2;
			}
			if (collisionMap->enemyCollisionDown(position, vampireSize, vampireHitboxOffset)) {
				position.y -= 2;
			}
			else if (!collisionMap->enemyCollisionDown(position + glm::ivec2(0, 2), vampireSize, vampireHitboxOffset)) {
				position.y += 2;
			}
		}
		else if (collisionMap->isBetweenCollisionsX(position, size, hitboxOffset)) {
			if (collisionMap->enemyCollisionUp(position + glm::ivec2(0, -2), size, hitboxOffset)) {
				if (direction == LEFT_UP) direction = LEFT_DOWN;
				else if (direction == RIGHT_UP) direction = RIGHT_DOWN;
			}
			else if (collisionMap->enemyCollisionDown(position + glm::ivec2(0, 2), size, hitboxOffset)) {
				if (direction == LEFT_DOWN) direction = LEFT_UP;
				else if (direction == RIGHT_DOWN) direction = RIGHT_UP;
			}
			if (direction == LEFT_UP || direction == RIGHT_UP) {
				position.y -= 2;
			}
			else {
				position.y += 2;
			}
			if (collisionMap->enemyCollisionLeft(position, glm::ivec2(48, 0), glm::ivec2(0, 24))) {
				position.x += 2;
			}
			else if (collisionMap->enemyCollisionRight(position, glm::ivec2(48, 0), glm::ivec2(0, 24))) {
				position.x -= 2;
			}
		}
		else {
			bool collisionLeft = (direction == LEFT_UP || direction == LEFT_DOWN) && collisionMap->enemyCollisionLeft(position + glm::ivec2(-2, 0), size, hitboxOffset);
			bool collisionRight = (direction == RIGHT_UP || direction == RIGHT_DOWN) && collisionMap->enemyCollisionRight(position + glm::ivec2(2, 0), size, hitboxOffset);
			bool collisionDown = (direction == LEFT_DOWN || direction == RIGHT_DOWN) && collisionMap->enemyCollisionDown(position + glm::ivec2(0, 2), size, hitboxOffset);
			bool collisionUp = (direction == LEFT_UP || direction == RIGHT_UP) && collisionMap->enemyCollisionUp(position + glm::ivec2(0, -2), size, hitboxOffset);
			if (collisionUp) {
				if (direction == RIGHT_UP) {
					direction = RIGHT_DOWN;
				}
				else if (direction == LEFT_UP) {
					direction = LEFT_DOWN;
				}
			}
			else if (collisionDown) {
				if (direction == RIGHT_DOWN) {
					direction = RIGHT_UP;
				}
				else if (direction == LEFT_DOWN) {
					direction = LEFT_UP;
				}
			}
			else if (collisionLeft) {
				sprite->changeAnimation(BAT_RIGHT);
				if (direction == LEFT_UP) {
					direction = RIGHT_UP;
				}
				else if (direction == LEFT_DOWN) {
					direction = RIGHT_DOWN;
				}
			}
			else if (collisionRight) {
				sprite->changeAnimation(BAT_LEFT);
				if (direction == RIGHT_UP) {
					direction = LEFT_UP;
				}
				else if (direction == RIGHT_DOWN) {
					direction = LEFT_DOWN;
				}
			}
			// MOVEMENT
			if (direction == LEFT_UP) {
				position.x -= 2;
				position.y -= 2;
			}
			else if (direction == LEFT_DOWN) {
				position.x -= 2;
				position.y += 2;
			}
			else if (direction == RIGHT_UP) {
				position.x += 2;
				position.y -= 2;
			}
			else if (direction == RIGHT_DOWN) {
				position.x += 2;
				position.y += 2;
			}
			
		}

		// TRANSFORMATION
		transformationTimeElapsed += deltaTime;
		if (transformationTimeElapsed >= 5000) {
			
			if (!collisionMap->enemyCollisionDown(position, vampireSize, vampireHitboxOffset) &&
				collisionMap->enemyCollisionDown(position + glm::ivec2(0, 2), vampireSize, vampireHitboxOffset) &&
				!collisionMap->isOnSpikes(position, vampireSize, vampireHitboxOffset)) {
				MediaPlayer* mediaPlayer = MediaPlayer::getInstance();
				mediaPlayer->playEnemySound("transformation");
				size = vampireSize;
				hitboxOffset = vampireHitboxOffset;
				transformationTimeElapsed = 0;
				bat = false;
				spawning = true;
				hitboxSprite->changeAnimation(0);
				spawnSprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
				if (sprite->animation() == BAT_LEFT) {
					sprite->changeAnimation(MOVE_LEFT);
				}
				else {
					sprite->changeAnimation(MOVE_RIGHT);
				}
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	hitboxSprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void Vampire::updateOnCollision() {
	if (!bat) {
		if (sprite->animation() == MOVE_RIGHT) {
			sprite->changeAnimation(MOVE_LEFT);
		}
		else
			sprite->changeAnimation(MOVE_RIGHT);
		MediaPlayer* mediaPlayer = MediaPlayer::getInstance();
		mediaPlayer->playEnemySound("vampire");
	}
	else {
		if (direction == RIGHT_UP) {
			direction = LEFT_DOWN;
		}
		else if (direction == RIGHT_DOWN) {
			direction = LEFT_UP;
		}
		else if (direction == LEFT_UP) {
			direction = RIGHT_DOWN;
		}
		else if (direction == LEFT_DOWN) {
			direction = RIGHT_UP;
		}
		if (direction == LEFT_DOWN || direction == LEFT_UP) {
			sprite->changeAnimation(BAT_LEFT);
		}
		else {
			sprite->changeAnimation(BAT_RIGHT);
		}
		MediaPlayer* mediaPlayer = MediaPlayer::getInstance();
		mediaPlayer->playEnemySound("bat");
	}
}