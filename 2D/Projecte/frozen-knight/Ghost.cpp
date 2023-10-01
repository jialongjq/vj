#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Ghost.h"
#include "Scene.h"

#define FALL_STEP 4

enum GhostAnims
{
	MOVE_RIGHT, MOVE_LEFT, TRANSPARENT_RIGHT, TRANSPARENT_LEFT
};


void Ghost::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	initHitbox(tileMapPos, shaderProgram, "ghost");
	hitboxOffset = glm::vec2(12, 0);
	spritesheet.loadFromFile("images/ghost.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE_RIGHT, 4);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0, 0.5f));

	sprite->setAnimationSpeed(TRANSPARENT_RIGHT, 4);
	sprite->addKeyframe(TRANSPARENT_RIGHT, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(TRANSPARENT_RIGHT, glm::vec2(0.5f, 0.5f));

	sprite->setAnimationSpeed(MOVE_LEFT, 4);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.5f));

	sprite->setAnimationSpeed(TRANSPARENT_LEFT, 4);
	sprite->addKeyframe(TRANSPARENT_LEFT, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(TRANSPARENT_LEFT, glm::vec2(0.75f, 0.5f));

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	sprite->changeAnimation(MOVE_RIGHT);

	tileMapDispl = tileMapPos;
}
void Ghost::setPlayer(Player* player) {
	this->player = player;
}
void Ghost::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (!player->isDead()) {
		glm::vec2 playerPos = player->getPosition();
		int previousAnimation = sprite->animation();
		int previousKeyframe = sprite->getCurrentKeyframe();
		float previousTimeAnimation = sprite->getTimeAnimation();
		if (playerPos.x < position.x) {
			if (sprite->animation() != MOVE_LEFT && sprite->animation() != TRANSPARENT_LEFT) {
				sprite->changeAnimation(MOVE_LEFT);
			}
			position.x -= 1;
		}
		else if (playerPos.x > position.x) {
			if (sprite->animation() != MOVE_RIGHT && sprite->animation() != TRANSPARENT_RIGHT) {
				sprite->changeAnimation(MOVE_RIGHT);
			}
			position.x += 1;
		}
		if (playerPos.y < position.y) {
			position.y -= 1;
		}
		else if (playerPos.y > position.y) {
			position.y += 1;
		}
		bool collision = (collisionMap->enemyCollisionDown(position, size, hitboxOffset) ||
			collisionMap->enemyCollisionLeft(position, size, hitboxOffset) ||
			collisionMap->enemyCollisionRight(position, size, hitboxOffset) ||
			collisionMap->enemyCollisionUp(position, size, hitboxOffset));
		if (sprite->animation() == MOVE_LEFT && collision) {
			sprite->changeAnimation(TRANSPARENT_LEFT);
		}
		else if (sprite->animation() == MOVE_RIGHT && collision) {
			sprite->changeAnimation(TRANSPARENT_RIGHT);
		}
		else if (sprite->animation() == TRANSPARENT_LEFT && !collision) {
			sprite->changeAnimation(MOVE_LEFT);
		}
		else if (sprite->animation() == TRANSPARENT_RIGHT && !collision) {
			sprite->changeAnimation(MOVE_RIGHT);
		}
		if (sprite->animation() != previousAnimation) {
			sprite->setCurrentKeyframe(previousKeyframe);
			sprite->setTimeAnimation(previousTimeAnimation);
		}
		hitboxSprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	}
}

void Ghost::updateOnCollision() {
	MediaPlayer* mediaPlayer = MediaPlayer::getInstance();
	mediaPlayer->playEnemySound("ghost");
}