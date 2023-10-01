#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#define JUMP_HEIGHT 74
#define FALL_STEP 4
#define SPEED 4

#define SCREEN_X 192
#define SCREEN_Y 96


enum PlayerAnims
{
	STAND_RIGHT, MOVE_RIGHT, JUMP_RIGHT, STAND_LEFT, MOVE_LEFT, JUMP_LEFT, DEATH_RIGHT, DEATH_LEFT
};

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	mediaPlayer = MediaPlayer::getInstance();

	initHitbox(tileMapPos, shaderProgram, "player");
	hitboxOffset = glm::vec2(12, 0);
	spawning = true;
	initSpawn(tileMapPos, shaderProgram);
	bJumping = false;
	hp = 3;
	shield = false;
	
	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);
		
		sprite->setAnimationSpeed(STAND_LEFT, 0);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.25f, 0.f));

		sprite->setAnimationSpeed(MOVE_LEFT, 6);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25, 0.0f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25, 0.50f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25, 0.0f));

		sprite->setAnimationSpeed(JUMP_LEFT, 0);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.25, 0.75f));

		sprite->setAnimationSpeed(STAND_RIGHT, 0);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 6);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.5f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));

		sprite->setAnimationSpeed(JUMP_RIGHT, 0);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.f, 0.75f));

		sprite->setAnimationSpeed(DEATH_RIGHT, 0);
		sprite->addKeyframe(DEATH_RIGHT, glm::vec2(0.5f, 0.0f));
		sprite->addKeyframe(DEATH_RIGHT, glm::vec2(0.5f, 0.25f));
		sprite->addKeyframe(DEATH_RIGHT, glm::vec2(0.5f, 0.5f));
		sprite->addKeyframe(DEATH_RIGHT, glm::vec2(0.5f, 0.75f));

		sprite->setAnimationSpeed(DEATH_LEFT, 0);
		sprite->addKeyframe(DEATH_LEFT, glm::vec2(0.75f, 0.0f));
		sprite->addKeyframe(DEATH_LEFT, glm::vec2(0.75f, 0.25f));
		sprite->addKeyframe(DEATH_LEFT, glm::vec2(0.75f, 0.5f));
		sprite->addKeyframe(DEATH_LEFT, glm::vec2(0.75f, 0.75f));
	
	sprite->changeAnimation(STAND_RIGHT);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	
}

void Player::update(int deltaTime)
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

	if (!isDead()) {
		// LATERAL MOVEMENT
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			position.x -= SPEED;
			if (collisionMap->collisionMoveLeft(position, size, hitboxOffset))
			{
				position.x += SPEED;
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			position.x += SPEED;
			if (collisionMap->collisionMoveRight(position, size, hitboxOffset))
			{
				position.x -= SPEED;
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else
		{
			if (sprite->animation() == MOVE_LEFT || sprite->animation() == JUMP_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == JUMP_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
		}
	}
	
	if (bJumping)
	{
		if (sprite->animation() != JUMP_RIGHT && sprite->animation() != JUMP_LEFT) {
			if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT)
				sprite->changeAnimation(JUMP_LEFT);
			else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT)
				sprite->changeAnimation(JUMP_RIGHT);
		}
		if (jumpHeight >= 176)
		{
			bJumping = false;
			position.y = startY;
		}
		else
		{
			if (jumpHeight < 88) {
				jumpHeight += 8;
				position.y -= 8;
			}
			else {
				jumpingTimeElapsed += deltaTime;
				if (jumpingTimeElapsed >= 100) {
					jumpHeight += 6;
					position.y += 6;
					bJumping = !collisionMap->collisionMoveDown(position, &position.y, size, hitboxOffset);
					if (!bJumping) {
						mediaPlayer->playDropSound();
					}
				}
			}
		}
	}
	else
	{
		jumpingTimeElapsed = 0;
		position.y += 6;
		if (collisionMap->collisionMoveDown(position, &position.y, size, hitboxOffset))
		{
			if (falling) {
				falling = false;
				mediaPlayer->playDropSound();
			}
			map->activateTile(position, size, hitboxOffset);
			if(!isDead() && Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				mediaPlayer->playJumpSound();
				bJumping = true;
				jumpHeight = 0;
				startY = position.y;
			}
		}
		else {
			falling = true;
			if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT) {
				sprite->changeAnimation(JUMP_LEFT);
			}
			else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT) {
				sprite->changeAnimation(JUMP_RIGHT);
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	hitboxSprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setCollisionMap(CollisionMap* collisionMap)
{
	this->collisionMap = collisionMap;
}

void Player::die(bool unconditional) {
	if (unconditional || !isDead()) {
		hp -= 1;
		int previousAnimation = sprite->animation();
		int previousKeyframe = sprite->getCurrentKeyframe();
		// DEATH LEFT OR RIGHT
		if (previousAnimation == MOVE_LEFT || previousAnimation == STAND_LEFT || previousAnimation == JUMP_LEFT) {
			sprite->changeAnimation(DEATH_LEFT);
		}
		else {
			sprite->changeAnimation(DEATH_RIGHT);
		}
		// KEEP THE SAME KEYFRAME TO GET THE FROZEN EFFECT
		if (previousAnimation == JUMP_LEFT || previousAnimation == JUMP_RIGHT) {
			sprite->setCurrentKeyframe(3);
		}
		else if (previousAnimation == STAND_LEFT || previousAnimation == STAND_RIGHT) {
			sprite->setCurrentKeyframe(0);
		}
		else {
			if (previousKeyframe == 0) {
				sprite->setCurrentKeyframe(1);
			}
			else if (previousKeyframe == 2) {
				sprite->setCurrentKeyframe(2);
			}
			else {
				sprite->setCurrentKeyframe(0);
			}
		}
	}
	
}

bool Player::isDead() {
	return (sprite->animation() == DEATH_LEFT) || (sprite->animation() == DEATH_RIGHT);
}

bool Player::isInvincible() {
	return invincible;
}

void Player::setInvincible(bool invincible) {
	this->invincible = invincible;
}

int Player::getHP() {
	return hp;
}

void Player::setHP(int hp) {
	this->hp = hp;
}

void Player::respawn() {
	spawning = true;
	position = spawnPosition;
	sprite->changeAnimation(STAND_RIGHT);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	hitboxSprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void Player::despawn() {
	spawning = true;
	spawnSprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

bool Player::isJumping() {
	return bJumping;
}

bool Player::isOnGround() {
	int savePosition = position.y;
	bool collideDown = collisionMap->collisionMoveDown(position + glm::ivec2(0, 4), &position.y, size, hitboxOffset);
	position.y = savePosition;
	return collideDown;
}

bool Player::hasShield() {
	return shield;
}

void Player::setShield(bool shield) {
	this->shield = shield;
}

void Player::renderHitbox() {
	if (showHitbox) {
		hitboxSprite->render();
	}
}