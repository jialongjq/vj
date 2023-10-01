#include "LevelUI.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Sprite.h"
#include<windows.h>

void LevelUI::init(glm::vec2 initialPos, ShaderProgram& shaderProgram)
{
	clear = false;
	ready = false;
	timeLimit = false;
	readyTimeElapsed = 0;
	gameStatus = 0;

	spritesheets.resize(7);

	//NUMBERS SPRITES
	spritesheets[0].loadFromFile("images/num.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[0].setMagFilter(GL_NEAREST);
	numbers.resize(11);
	numbers2.resize(11); //hearts(1), score(6), time(2), stage(2)
	glm::vec2 actualPos = glm::vec2(initialPos.x + 50, initialPos.y - 50);
	Sprite* sprite;
	for (int i = 0; i < 11; ++i) {
		sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(0.1, 0.5), &spritesheets[0], &shaderProgram);
		sprite->setNumberAnimations(10);

		sprite->setAnimationSpeed(0, 4);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));

		sprite->setAnimationSpeed(1, 4);
		sprite->addKeyframe(1, glm::vec2(0.1, 0.f));

		sprite->setAnimationSpeed(2, 4);
		sprite->addKeyframe(2, glm::vec2(0.2f, 0.f));

		sprite->setAnimationSpeed(3, 4);
		sprite->addKeyframe(3, glm::vec2(0.3f, 0.f));

		sprite->setAnimationSpeed(4, 4);
		sprite->addKeyframe(4, glm::vec2(0.4f, 0.f));

		sprite->setAnimationSpeed(5, 4);
		sprite->addKeyframe(5, glm::vec2(0.5f, 0.f));

		sprite->setAnimationSpeed(6, 4);
		sprite->addKeyframe(6, glm::vec2(0.6f, 0.f));

		sprite->setAnimationSpeed(7, 4);
		sprite->addKeyframe(7, glm::vec2(0.7f, 0.f));

		sprite->setAnimationSpeed(8, 4);
		sprite->addKeyframe(8, glm::vec2(0.8f, 0.f));

		sprite->setAnimationSpeed(9, 4);
		sprite->addKeyframe(9, glm::vec2(0.9f, 0.f));

		sprite->changeAnimation(0);
		sprite->setPosition(actualPos);
		numbers[i] = sprite;
		if (i == 0 || i == 6) actualPos.x += 18 * (i + 1);
		else if (i == 8) actualPos.x += 38 * i;
		actualPos.x += 24;
	}

	actualPos = glm::vec2(554, 46);
	for (int i = 0; i < 2; ++i) {
		sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(0.1, 0.5), &spritesheets[0], &shaderProgram);
		sprite->setNumberAnimations(10);

		sprite->setAnimationSpeed(0, 4);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.5f));

		sprite->setAnimationSpeed(1, 4);
		sprite->addKeyframe(1, glm::vec2(0.1, 0.5f));

		sprite->setAnimationSpeed(2, 4);
		sprite->addKeyframe(2, glm::vec2(0.2f, 0.5f));

		sprite->setAnimationSpeed(3, 4);
		sprite->addKeyframe(3, glm::vec2(0.3f, 0.5f));

		sprite->setAnimationSpeed(4, 4);
		sprite->addKeyframe(4, glm::vec2(0.4f, 0.5f));

		sprite->setAnimationSpeed(5, 4);
		sprite->addKeyframe(5, glm::vec2(0.5f, 0.5f));

		sprite->setAnimationSpeed(6, 4);
		sprite->addKeyframe(6, glm::vec2(0.6f, 0.5f));

		sprite->setAnimationSpeed(7, 4);
		sprite->addKeyframe(7, glm::vec2(0.7f, 0.5f));

		sprite->setAnimationSpeed(8, 4);
		sprite->addKeyframe(8, glm::vec2(0.8f, 0.5f));

		sprite->setAnimationSpeed(9, 4);
		sprite->addKeyframe(9, glm::vec2(0.9f, 0.5f));

		sprite->changeAnimation(0);

		sprite->setPosition(actualPos);
		numbers2[i] = sprite;

		actualPos.x += 24;
	}

	//heart
	images.resize(7);
	spritesheets[1].loadFromFile("images/heart.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[1].setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(48, 24), glm::vec2(1, 0.5), &spritesheets[1], &shaderProgram);
	sprite->setNumberAnimations(2);
	sprite->setAnimationSpeed(0, 0);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->setAnimationSpeed(1, 0);
	sprite->addKeyframe(1, glm::vec2(0.f, 0.5f));
	sprite->setPosition(glm::vec2(initialPos.x, initialPos.y - 50));

	images[0] = sprite;


	//stage
	spritesheets[2].loadFromFile("images/stage.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[2].setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(120, 24), glm::vec2(1, 1), &spritesheets[2], &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->setPosition(glm::vec2(initialPos.x + 44 * 13, initialPos.y - 50));

	images[1] = sprite;

	//clear statge
	spritesheets[3].loadFromFile("images/clear.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[3].setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(288, 27), glm::vec2(1, 1), &spritesheets[3], &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->setPosition(glm::vec2(initialPos.x + 238, initialPos.y + 240));

	images[2] = sprite;


	//ready?
	spritesheets[4].loadFromFile("images/ready.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[4].setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(147, 27), glm::vec2(1, 1), &spritesheets[4], &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->setPosition(glm::vec2(initialPos.x + 310, initialPos.y + 240));

	images[3] = sprite;

	//game over
	spritesheets[5].loadFromFile("images/game_over.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[5].setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(240, 24), glm::vec2(1, 1), &spritesheets[5], &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->setPosition(glm::vec2(initialPos.x + 250, initialPos.y + 240));

	images[4] = sprite;

	//game completed
	spritesheets[6].loadFromFile("images/game_completed.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[6].setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(336, 24), glm::vec2(1, 1), &spritesheets[6], &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->setPosition(glm::vec2(initialPos.x + 200, initialPos.y + 240));

	images[5] = sprite;
}



void LevelUI::update(int deltaTime)
{
	readyTimeElapsed += deltaTime;
	if (readyTimeElapsed >= 2000) {
		ready = true;
	}
}

void LevelUI::updateShield(bool shield) {
	if (shield) {
		images[0]->changeAnimation(1);
	}
	else {
		images[0]->changeAnimation(0);
	}
}

void LevelUI::updateHP(int hp) {
	numbers[0]->changeAnimation(hp);
}

void LevelUI::updateStage(int stage) {
	numbers[9]->changeAnimation(int(stage / 10));
	numbers[10]->changeAnimation(stage % 10);
}

void LevelUI::updateTime(int time) {
	if (time <= 60) {
		if (!clear && time <= 10) {
			timeLimit = true;
			numbers2[0]->changeAnimation(int(time / 10) % 10);
			numbers2[1]->changeAnimation(int(time) % 10);
		}
		else {
			if (timeLimit) timeLimit = false;
			numbers[7]->changeAnimation(int(time / 10) % 10);
			numbers[8]->changeAnimation(int(time) % 10);
		}
	}
}

void LevelUI::updateScore(int score) {
	numbers[1]->changeAnimation((score / 100000) % 10);
	numbers[2]->changeAnimation((score / 10000) % 10);
	numbers[3]->changeAnimation((score / 1000) % 10);
	numbers[4]->changeAnimation((score / 100) % 10);
	numbers[5]->changeAnimation((score / 10) % 10);
	numbers[6]->changeAnimation(score % 10);
}


void LevelUI::updateGameStatus(int gameStatus)
{
	this->gameStatus = gameStatus;
}


void LevelUI::render()
{
	for (int i = 0; i < 11; ++i) {
		if ((i != 7 && i != 8) || !timeLimit) numbers[i]->render();
	}
	if (timeLimit) {
		numbers2[0]->render();
		numbers2[1]->render();
	}
	//heart & stage
	images[0]->render();
	images[1]->render();

	//game completed
	if (gameStatus == 2) images[5]->render();
	else {
		//game over
		if (gameStatus == 1) images[4]->render();
		//statge clear
		if (clear) {
			images[2]->render();
		}
	}
	//ready?
	if (!ready) images[3]->render();
}


void LevelUI::setClear(bool clear)
{
	this->clear = clear;
}

bool LevelUI::isReady() {
	return ready;
}