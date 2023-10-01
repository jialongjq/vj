#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include "MainMenu.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25


MainMenu::MainMenu()
{
	menuUI = NULL;
}

MainMenu::~MainMenu()
{

	if (menuUI != NULL)
		delete menuUI;
}


void MainMenu::reshape(int w, int h) {
	float scale = h / 720.f;
	float offset = (w - 24 * 48 * scale) / 2;

	projection = glm::ortho(-offset, w - offset, float(h), 0.f);
	projection = glm::scale(projection, glm::vec3(scale));
}


void MainMenu::init()
{
	initShaders();
	menuUI = new MenuUI();
	menuUI->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	arrow = 0;
	mediaPlayer = MediaPlayer::getInstance();
	mediaPlayer->stopAll();
	mediaPlayer->playTitleMusic();
}

void MainMenu::updateArrow(int action) {
	if (!menuUI->inCredits() && !menuUI->inInstructions()) {
		arrow += action;
		if (arrow == -1) {
			arrow = 2;
		}
		else if (arrow == 3) {
			arrow = 0;
		}
		menuUI->updateStatus(arrow);
	}
}

void MainMenu::update(int deltaTime)
{
	currentTime += deltaTime;
	menuUI->update(deltaTime);
}

int MainMenu::getStatus() {
	return arrow;
}

bool MainMenu::inCredits() {
	return menuUI->inCredits();
}

bool MainMenu::inInstructions() {
	return menuUI->inInstructions();
}

int MainMenu::nextAction() {
	menuUI->nextAction(arrow);
	return arrow;
}
void MainMenu::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	menuUI->render();
}

void MainMenu::initShaders()
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

