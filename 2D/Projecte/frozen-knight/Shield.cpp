#include "Shield.h"
#include <iostream>
using namespace std;

void Shield::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	initHitbox(tileMapPos, shaderProgram, "shield");
	hitboxOffset = glm::vec2(10, 6);
	initSpawn(tileMapPos, shaderProgram);
	status = HIDDEN;

	spritesheet.loadFromFile("images/shield.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(1.0, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 2);
	sprite->addKeyframe(0, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(0, glm::vec2(0.0f, 0.5f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

}