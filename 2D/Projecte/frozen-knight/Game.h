#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "MainMenu.h"

#define SCREEN_WIDTH 1152
#define SCREEN_HEIGHT 720


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game() {}
	
public:
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();

	void reshape(int w, int h);
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

private:
	int status;
	int level;
	bool bPlay;                       // Continue to play game?
	Scene* scene;                      // Scene to render
	MainMenu* mainMenu;
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	bool canBePressed[256] = { 1 }, canBePressedSpecial[256] = { 1 }; // indicates if certain keys are pending to be treated
	ShaderProgram texProgram;
	MediaPlayer* mediaPlayer;
	void initShaders();
};


#endif // _GAME_INCLUDE


