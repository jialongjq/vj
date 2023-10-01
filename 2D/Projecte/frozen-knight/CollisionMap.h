#ifndef _COLLISION_MAP_INCLUDE
#define _COLLISION_MAP_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"

// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.

class Scene;

class CollisionMap
{

private:
	CollisionMap(const string &levelFile);

public:
	// Tile maps can only be created inside an OpenGL context
	static CollisionMap *createCollisionMap(const string &levelFile);
	~CollisionMap();
	
	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, const glm::ivec2& hitboxOffset) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, const glm::ivec2& hitboxOffset) const;
	bool collisionMoveDown(const glm::ivec2 &pos, int* posY, const glm::ivec2 &size, const glm::ivec2& hitboxOffset) const;

	bool enemyCollisionDown(const glm::ivec2& pos, const glm::ivec2& size, const glm::ivec2& hitboxOffset) const;
	bool enemyCollisionUp(const glm::ivec2& pos, const glm::ivec2& size, const glm::ivec2& hitboxOffset) const;
	bool enemyCollisionLeft(const glm::ivec2& pos, const glm::ivec2& size, const glm::ivec2& hitboxOffset) const;
	bool enemyCollisionRight(const glm::ivec2& pos, const glm::ivec2& size, const glm::ivec2& hitboxOffset) const;

	bool isOnSpikes(const glm::ivec2& pos, const glm::ivec2& size, const glm::ivec2& hitboxOffset) const;

	bool isBetweenCollisionsX(const glm::ivec2& pos, const glm::ivec2& size, const glm::ivec2& hitboxOffset) const;
	bool isBetweenCollisionsY(const glm::ivec2& pos, const glm::ivec2& size, const glm::ivec2& hitboxOffset) const;

	void setScene(Scene* scene);

private:
	bool loadCollisions(const string &levelFile);

private:
	int nTiles;
	int tileSize;
	glm::ivec2 mapSize;
	Scene* scene;
	int *map;
};

#endif