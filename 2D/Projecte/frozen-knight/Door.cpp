#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Door.h"
#include "Game.h"

enum DoorAnims
{
	LOCKED, UNLOCKED
};


void Door::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	initHitbox(tileMapPos, shaderProgram, "door");
	hitboxOffset = glm::vec2(8, 0);

	spritesheet.loadFromFile("images/door.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(1, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(LOCKED, 0);
	sprite->addKeyframe(LOCKED, glm::vec2(1.f, 0.0f));

	sprite->setAnimationSpeed(UNLOCKED, 0);
	sprite->addKeyframe(UNLOCKED, glm::vec2(1.f, 0.5f));


	sprite->changeAnimation(LOCKED);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void Door::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Door::unlock() {
	if (sprite->animation() == LOCKED) {
		sprite->changeAnimation(UNLOCKED);
	}
}

bool Door::isUnlocked() {
	return sprite->animation() == UNLOCKED;
}