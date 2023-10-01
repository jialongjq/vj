#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Key.h"
#include "Game.h"

void Key::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	mediaPlayer = MediaPlayer::getInstance();
	initHitbox(tileMapPos, shaderProgram, "key");
	hitboxOffset = glm::vec2(16, 10);
	initSpawn(tileMapPos, shaderProgram);
	status = HIDDEN;

	spritesheet.loadFromFile("images/key.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(1, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 2);
	sprite->addKeyframe(0, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(0, glm::vec2(0.0f, 0.5f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

}

void Key::update(int deltaTime) {
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
}

void Key::show() {
	if (status == HIDDEN) {
		status = SHOWN;
		spawning = true;
		mediaPlayer->playCollectableShownSound();
	}
}

void Key::collect() {
	if (status == SHOWN) {
		status = COLLECTED;
		spawning = false;
		door->unlock();
		mediaPlayer->playUnlockSound();
	}
}

void Key::setDoor(Door* door) {
	this->door = door;
}