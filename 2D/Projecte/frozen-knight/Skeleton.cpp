#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Skeleton.h"

#define FALL_STEP 4

enum SkeletonAnims
{
	MOVE_RIGHT, MOVE_LEFT
};


void Skeleton::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	initHitbox(tileMapPos, shaderProgram, "skeleton");
	hitboxOffset = glm::vec2(12, 0);
	spritesheet.loadFromFile("images/skeleton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(0.5, 1/3.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(MOVE_RIGHT, 4);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 1/3.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 2 / 3.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 4);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5, 1/3.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5, 2/3.f));

	sprite->changeAnimation(MOVE_RIGHT);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void Skeleton::update(int deltaTime)
{
	sprite->update(deltaTime);

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
	if (sprite->animation() == MOVE_RIGHT) {
		position.x += 2;
	}
	else {
		position.x -= 2;
	}
	hitboxSprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void Skeleton::updateOnCollision() {
	MediaPlayer* mediaPlayer = MediaPlayer::getInstance();
	mediaPlayer->playEnemySound("skeleton");

	if (sprite->animation() == MOVE_RIGHT) {
		sprite->changeAnimation(MOVE_LEFT);
	}
	else
		sprite->changeAnimation(MOVE_RIGHT);
}