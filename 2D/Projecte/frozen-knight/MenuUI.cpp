#include "MenuUI.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Sprite.h"
#include<windows.h>

void MenuUI::init(glm::vec2 pos, ShaderProgram& shaderProgram)
{
	instructions = false;
	credits = false;
	readyTimeElapsed = 0;


	spritesheets.resize(8);
	images.resize(10);

	//Frozen
	spritesheets[0].loadFromFile("images/FrozenIce.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[0].setMagFilter(GL_NEAREST);
	Sprite* sprite = Sprite::createSprite(glm::ivec2(720, 240), glm::vec2(1, 1), &spritesheets[0], &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->setPosition(glm::vec2(pos.x + 200, pos.y + 80));

	images[0] = sprite;

	//Start game
	spritesheets[1].loadFromFile("images/start.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[1].setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(120, 24), glm::vec2(1, 1), &spritesheets[1], &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->setPosition(glm::vec2(pos.x + 450, pos.y + 360));

	images[1] = sprite;


	//Instructions
	spritesheets[2].loadFromFile("images/instructions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[2].setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(288, 24), glm::vec2(1, 1), &spritesheets[2], &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->setPosition(glm::vec2(pos.x + 450, pos.y + 420));

	images[2] = sprite;

	//Credits
	spritesheets[3].loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[3].setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(168, 24), glm::vec2(1, 1), &spritesheets[3], &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->setPosition(glm::vec2(pos.x + 450, pos.y + 480));

	images[3] = sprite;

	//Arrow
	arrowPos = glm::vec2(pos.x + 390, pos.y + 360);
	spritesheets[4].loadFromFile("images/arrow.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[4].setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(56, 24), glm::vec2(1, 0.5f), &spritesheets[4], &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 2);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.0f, 0.5f));

	sprite->changeAnimation(0);
	sprite->setPosition(arrowPos);

	images[4] = sprite;

	//Screen instructions
	sprite = Sprite::createSprite(glm::ivec2(480, 40), glm::vec2(1, 1), &spritesheets[2], &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->setPosition(glm::vec2(pos.x + 300, pos.y + 50));
	images[5] = sprite;

	spritesheets[5].loadFromFile("images/screen_instructions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[5].setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(700, 525), glm::vec2(1, 1), &spritesheets[5], &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->setPosition(glm::vec2(pos.x + 160, pos.y + 180));

	images[6] = sprite;

	spritesheets[6].loadFromFile("images/instructions_text.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[6].setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(900, 100), glm::vec2(1, 1), &spritesheets[6], &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->setPosition(glm::vec2(pos.x + 110, pos.y+120));

	images[7] = sprite;

	//Screen credits
	sprite = Sprite::createSprite(glm::ivec2(280, 40), glm::vec2(1, 1), &spritesheets[3], &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->setPosition(glm::vec2(pos.x + 400, pos.y + 50));
	images[8] = sprite;

	spritesheets[7].loadFromFile("images/screen_credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheets[7].setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(800, 600), glm::vec2(1, 1), &spritesheets[7], &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->setPosition(glm::vec2(pos.x + 140, pos.y + 80));

	images[9] = sprite;
	this->pos = pos;
}



void MenuUI::update(int deltaTime)
{
	readyTimeElapsed += deltaTime;
	if (readyTimeElapsed >= 2000) {
		ready = true;
	}
	images[4]->update(deltaTime);
}

void MenuUI::updateStatus(int arrow) {
	arrowPos.x = pos.x + 390;
	arrowPos.y = pos.y + 360 + arrow * 60;
	images[4]->setPosition(arrowPos);
}

void MenuUI::render()
{
	if (instructions) {
		images[5]->render();
		images[6]->render();
		images[7]->render();
	}
	else if (credits) {
		images[9]->render();
		images[8]->render();
	}
	else {
		for (int i = 0; i < 5; ++i) {
			images[i]->render();
		}
	}
}


void MenuUI::nextAction(int status) {
	if (status == 1) {
		instructions = !instructions;
		//if (instructions) images[2]->setPosition(glm::vec2(pos.x + 380, pos.y + 50));
		//else images[2]->setPosition(glm::vec2(pos.x + 450, pos.y + 420));

	}
	else if (status == 2) {
		credits = !credits;
		if (credits) images[3]->setPosition(glm::vec2(pos.x + 400, pos.y + 100));
		else images[3]->setPosition(glm::vec2(pos.x + 450, pos.y + 480));
	}

}

bool MenuUI::inInstructions() {
	return instructions;
}
bool MenuUI::inCredits() {
	return credits;
}