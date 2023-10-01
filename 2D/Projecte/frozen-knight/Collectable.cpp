#include "Collectable.h"
#include <iostream>
#include <random>
using namespace std;

void Collectable::update(int deltaTime) {
	if (spawning) {
		spawnSprite->update(deltaTime);
		spawningTimeElapsed += deltaTime;

		if (spawningTimeElapsed >= spawningTime) {
			spawning = false;
			spawningTimeElapsed = 0;
		}
		return;
	}
	if (status == SHOWN) {
		shownTimeElapsed += deltaTime;
		if (shownTimeElapsed >= 7500) {
			blinkingTimeElapsed += deltaTime;
			if (blinkingTimeElapsed >= 100) {
				blinkingTimeElapsed -= 100;
				renderCollectable = !renderCollectable;
			}
		}
		// Collectable disappears after 10 seconds
		if (shownTimeElapsed >= 10000) {
			status = COLLECTED;
		}
	}
	sprite->update(deltaTime);
}

void Collectable::render()
{
	if (status == SHOWN) {
		if (spawning) {
			spawnSprite->render();
		}
		else {
			if (renderCollectable) sprite->render();
		}
		if (showHitbox)
			hitboxSprite->render();
	}
}

void Collectable::show() {
	if (status == HIDDEN) {
		random_device rd;  // Will be used to obtain a seed for the random number engine
		mt19937 gen(rd());
		uniform_int_distribution<int> distribution(1, 100);
		int number = distribution(gen);
		if (number <= 5) {
			status = SHOWN;
			spawning = true;
			if (!mediaPlayer) {
				mediaPlayer = MediaPlayer::getInstance();
			}
			mediaPlayer->playCollectableShownSound();
		}
	}
}

void Collectable::collect() {
	if (status == SHOWN) {
		status = COLLECTED;
		spawning = false;
		if (!mediaPlayer) {
			mediaPlayer = MediaPlayer::getInstance();
		}
		mediaPlayer->playCollectableCollectedSound();
	}
}

bool Collectable::isCollectable() {
	return status == SHOWN;
}

int Collectable::getValue() {
	return 0;
}