#include "Gem.h"
#include <iostream>
using namespace std;

enum Color {
	GREEN, BLUE, RED, PURPLE
};

void Gem::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	initHitbox(tileMapPos, shaderProgram, "gem");
	hitboxOffset = glm::vec2(14, 10);
	initSpawn(tileMapPos, shaderProgram);
	status = HIDDEN;

	spritesheet.loadFromFile("images/gem.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	random_device rd;  // Will be used to obtain a seed for the random number engine
	mt19937 gen(rd());
	uniform_int_distribution<int> distribution(0, 3);
	float number = float(distribution(gen));
	color = int(number);
	if (color == GREEN) {
		value = 10;
	}
	else if (color == BLUE) {
		value = 50;
	}
	else if (color == RED) {
		value = 200;
	}
	else {
		value = 500;
	}

	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(number/4, 0.0f));
	sprite->addKeyframe(0, glm::vec2(number/4, 0.25f));
	sprite->addKeyframe(0, glm::vec2(number/4, 0.5f));
	sprite->addKeyframe(0, glm::vec2(number/4, 0.75f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

}

int Gem::getValue() {
	return value;
}