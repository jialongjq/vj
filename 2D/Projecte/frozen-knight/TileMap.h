#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "Key.h"
#include <vector>
#include <map>
#include <string>


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.

class Scene;

class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }

	void activateTile(glm::ivec2& pos, const glm::ivec2& size, const glm::ivec2& hitboxOffset);

	void setKey(Key* key);
	void setScene(Scene* scene);
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(ShaderProgram &program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
	int *indices;
	vector<float> vertices;
	glm::vec2 minCoords;
	int nActivable;
	Key *key;
	Scene* scene;
};


#endif // _TILE_MAP_INCLUDE


