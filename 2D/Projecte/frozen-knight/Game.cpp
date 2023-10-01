#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include "Game.h"
#include <irrKlang.h>

enum Status {
	MENU, PLAYING
};

void Game::init()
{
	mediaPlayer = MediaPlayer::getInstance();
	for (int i = 0; i < 256; i++) {
		canBePressed[i] = true;
		canBePressedSpecial[i] = true;
	}
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	level = 1;
	initShaders();
	status = MENU;
	scene = new Scene();
	scene->init(level, texProgram, 0, 3);
	mainMenu = new MainMenu();
	mainMenu->init();
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	reshape(width, height);
}

bool Game::update(int deltaTime)
{
	if (status == MENU) {
		mainMenu->update(deltaTime);
		if (specialKeys[GLUT_KEY_UP]) {
			if (!mainMenu->inCredits() && !mainMenu->inInstructions()) {
				mediaPlayer->playArrowSound();
			}
			specialKeys[GLUT_KEY_UP] = false;
			mainMenu->updateArrow(-1);

		}
		else if (specialKeys[GLUT_KEY_DOWN]) {
			if (!mainMenu->inCredits() && !mainMenu->inInstructions()) {
				mediaPlayer->playArrowSound();

			}
			specialKeys[GLUT_KEY_DOWN] = false;
			mainMenu->updateArrow(1);
		}
		else if (keys[32]) {
			keys[32] = false;
			if (mainMenu->getStatus() == 0) {
				mediaPlayer->playOpenSound();
				status = PLAYING;
				scene = new Scene();
				int width = glutGet(GLUT_WINDOW_WIDTH);
				int height = glutGet(GLUT_WINDOW_HEIGHT);
				reshape(width, height);
				level = 1;
				scene->init(level, texProgram, 0, 3);
			}
			else if (!mainMenu->inCredits() && !mainMenu->inInstructions()) {
				mediaPlayer->playOpenSound();
				mainMenu->nextAction();
			}
		}
		else if (keys[27]) {
			keys[27] = false;
			mediaPlayer->playExitSound();
			if (!mainMenu->inCredits() && !mainMenu->inInstructions()) {
				bPlay = false;
			}
			else {
				mainMenu->nextAction();
			}
		}
	}
	else {
		if (scene->isFinished()) {
			if (!scene->isGameOver()) {
				int levelScore = scene->getScore();
				int playerHP = scene->getPlayerHP();
				level += 1;
				if (level <= 3) {
					scene = new Scene();
					int width = glutGet(GLUT_WINDOW_WIDTH);
					int height = glutGet(GLUT_WINDOW_HEIGHT);
					reshape(width, height);
					scene->init(level, texProgram, levelScore, playerHP);
				}
				else {
					status = MENU;
					mediaPlayer->playTitleMusic();
				}
			}
			else {
				status = MENU;
				mediaPlayer->playTitleMusic();
			}
		}

		// Key 1
		if (keys['1']) {
			keys['1'] = false;
			scene = new Scene();
			int width = glutGet(GLUT_WINDOW_WIDTH);
			int height = glutGet(GLUT_WINDOW_HEIGHT);
			reshape(width, height);
			level = 1;
			scene->init(level, texProgram, 0, 3);
		}
		// Key 2
		if (keys['2']) {
			keys['2'] = false;
			scene = new Scene();
			int width = glutGet(GLUT_WINDOW_WIDTH);
			int height = glutGet(GLUT_WINDOW_HEIGHT);
			reshape(width, height);
			level = 2;
			scene->init(level, texProgram, 0, 3);
		}
		// Key 3
		if (keys['3']) {
			keys['3'] = false;
			scene = new Scene();
			int width = glutGet(GLUT_WINDOW_WIDTH);
			int height = glutGet(GLUT_WINDOW_HEIGHT);
			reshape(width, height);
			level = 3;
			scene->init(level, texProgram, 0, 3);
		}
		// Key G / g
		if (keys['g'] || keys['G']) {
			keys['g'] = false;
			keys['G'] = false;
			scene->changeInvulnerability();
			mediaPlayer->playInvulnerabilitySound();
		}
		// Key K / k
		if (keys['K'] || keys['k']) {
			keys['K'] = false;
			keys['k'] = false;
			scene->showKey();
		}
		if (keys['B'] || keys['b']) {
			keys['b'] = false;
			keys['B'] = false;
			scene->changeHitboxVisibility();
		}
		if (keys[27]) {
			mediaPlayer->stopAll();
			mediaPlayer->playExitSound();
			keys[27] = false;
			status = MENU;
			mediaPlayer->playTitleMusic();
		}
		scene->update(deltaTime);
	}
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (status == PLAYING) {
		scene->render();
	}
	else {
		mainMenu->render();
	}
}

void Game::keyPressed(int key)
{
	if ((key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z')) {
		int otherKey = key;
		if (key >= 'a') {
			otherKey -= 32;
		}
		else {
			otherKey += 32;
		}
		if (canBePressed[key] && canBePressed[otherKey]) {
			canBePressed[key] = false;
			canBePressed[otherKey] = false;
			keys[key] = true;
			keys[otherKey] = true;
		}
	}
	else if (key >= '1' && key <= '3') {
		if (canBePressed[key]) {
			canBePressed[key] = false;
			keys[key] = true;
		}
	}
	else if (key == 32 || key == 27) {
		if (canBePressed[key]) {
			canBePressed[key] = false;
			keys[key] = true;
		}
	}
	else {
		keys[key] = true;
	}
}

void Game::keyReleased(int key)
{
	if ((key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z')) {
		int otherKey = key;
		if (key >= 'a') {
			otherKey -= 32;
		}
		else {
			otherKey += 32;
		}
		keys[key] = false;
		keys[otherKey] = false;
		canBePressed[key] = true;
		canBePressed[otherKey] = true;
	}
	else if (key >= '1' && key <= '3') {
		keys[key] = false;
		canBePressed[key] = true;
	}
	else {
		keys[key] = false;
		canBePressed[key] = true;
	}
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::reshape(int w, int h) {
	scene->reshape(w, h);
	mainMenu->reshape(w, h);
}

void Game::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

