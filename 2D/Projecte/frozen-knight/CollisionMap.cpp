#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "CollisionMap.h"
#include "Scene.h"

using namespace std;

CollisionMap *CollisionMap::createCollisionMap(const string &levelFile)
{
	
	CollisionMap *map = new CollisionMap(levelFile);
	return map;
}


CollisionMap::CollisionMap(const string &levelFile)
{
	loadCollisions(levelFile);
}

CollisionMap::~CollisionMap()
{
	if (map != NULL)
		delete map;
}

bool CollisionMap::loadCollisions(const string &levelFile)
{
	ifstream fin;
	string line;
	stringstream sstream;
	char tile;
	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	while (line.compare(0, 12, "COLLISIONMAP") != 0) {
		getline(fin, line);
	}
	if (line.compare(0, 12, "COLLISIONMAP") != 0) {
		return false;
	}

	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;

	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize;

	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			fin.get(tile);
			if (tile == ' ')
				map[j * mapSize.x + i] = 0;
			else
				map[j * mapSize.x + i] = tile - '0';
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	
	return true;
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool CollisionMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, const glm::ivec2& hitboxOffset) const
{
	int x, y0, y1;

	x = (pos.x + hitboxOffset.x) / tileSize;
	y0 = (pos.y + hitboxOffset.y) / tileSize;
	y1 = (pos.y + hitboxOffset.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] == 1) {
			return true;
		}
	}

	return false;
}

bool CollisionMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, const glm::ivec2& hitboxOffset) const
{
	int x, y0, y1;

	x = (pos.x + hitboxOffset.x + size.x - 1) / tileSize;
	y0 = (pos.y + hitboxOffset.y) / tileSize;
	y1 = (pos.y + hitboxOffset.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] == 1)
			return true;
	}

	return false;
}

bool CollisionMap::collisionMoveDown(const glm::ivec2 &pos, int* posY, const glm::ivec2 &size, const glm::ivec2& hitboxOffset) const
{
	int x0, x1, y;

	x0 = (pos.x + hitboxOffset.x) / tileSize;
	x1 = (pos.x + hitboxOffset.x + size.x - 1) / tileSize;
	y = (pos.y + hitboxOffset.y + size.y - 1) / tileSize;

	// check for spikes
	for (int x = x0; x <= x1; x++) {
		if (map[y * mapSize.x + x] == 3) {
			scene->killPlayer(false);
		}
	}

	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0)
		{
			if (*posY - tileSize * y + size.y <= 6)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}
	return false;
}

bool CollisionMap::enemyCollisionLeft(const glm::ivec2& pos, const glm::ivec2& size, const glm::ivec2& hitboxOffset) const
{
	int x, y0, y1;

	x = (pos.x + hitboxOffset.x) / tileSize;
	y0 = (pos.y + hitboxOffset.y) / tileSize;
	y1 = (pos.y + hitboxOffset.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] != 0) {
			return true;
		}
	}
	return false;
}

bool CollisionMap::enemyCollisionRight(const glm::ivec2& pos, const glm::ivec2& size, const glm::ivec2& hitboxOffset) const
{
	int x, y0, y1;

	x = (pos.x + hitboxOffset.x + size.x - 1) / tileSize;
	y0 = (pos.y + hitboxOffset.y) / tileSize;
	y1 = (pos.y + hitboxOffset.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] != 0)
			return true;
	}

	return false;
}

bool CollisionMap::enemyCollisionDown(const glm::ivec2& pos, const glm::ivec2& size, const glm::ivec2& hitboxOffset) const
{
	int x0, x1, y;

	x0 = (pos.x + hitboxOffset.x) / tileSize;
	x1 = (pos.x + hitboxOffset.x + size.x - 1) / tileSize;
	y = (pos.y + hitboxOffset.y + size.y - 1) / tileSize;

	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0)
		{
			return true;
		}
	}
	return false;
}

bool CollisionMap::enemyCollisionUp(const glm::ivec2& pos, const glm::ivec2& size, const glm::ivec2& hitboxOffset) const
{
	int x0, x1, y;
	x0 = (pos.x + hitboxOffset.x) / tileSize;
	x1 = (pos.x + hitboxOffset.x + size.x - 1) / tileSize;
	y = (pos.y + hitboxOffset.y) / tileSize;

	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0)
		{
			return true;
		}
	}
	return false;
}

bool CollisionMap::isBetweenCollisionsY(const glm::ivec2& pos, const glm::ivec2& size, const glm::ivec2& hitboxOffset) const
{
	int x0, x1, y0, y1;

	x0 = (pos.x + hitboxOffset.x) / tileSize;
	x1 = (pos.x + hitboxOffset.x + size.x - 1) / tileSize;

	y0 = (pos.y + hitboxOffset.y) / tileSize;
	y1 = (pos.y + hitboxOffset.y + size.y - 1) / tileSize;

	bool collisionUp = false;
	bool collisionDown = false;
	for (int x = x0; x <= x1; x++)
	{
		if (map[(y0 - 1) * mapSize.x + x] != 0) {
			collisionUp = true;
		}
		if (map[(y1 + 1) * mapSize.x + x] != 0) {
			collisionDown = true;
		}
		if (collisionUp && collisionDown) {
			return true;
		}
		collisionUp = false;
		collisionDown = false;
	}
	return false;
}

bool CollisionMap::isBetweenCollisionsX(const glm::ivec2& pos, const glm::ivec2& size, const glm::ivec2& hitboxOffset) const
{
	int x0, x1, y0, y1;

	x0 = (pos.x + hitboxOffset.x) / tileSize;
	x1 = (pos.x + hitboxOffset.x + size.x - 1) / tileSize;

	y0 = (pos.y + hitboxOffset.y) / tileSize;
	y1 = (pos.y + hitboxOffset.y + size.y - 1) / tileSize;

	bool collisionLeft = false;
	bool collisionRight = false;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + (x0 - 1)] != 0) {
			collisionLeft = true;
		}
		if (map[y * mapSize.x + (x1 + 1)] != 0) {
			collisionRight = true;
		}
		if (collisionLeft && collisionRight) {
			return true;
		}
		collisionLeft = false;
		collisionRight = false;
	}
	return false;
}

bool CollisionMap::isOnSpikes(const glm::ivec2& pos, const glm::ivec2& size, const glm::ivec2& hitboxOffset) const
{
	int x0, x1, y;

	x0 = (pos.x + hitboxOffset.x) / tileSize;
	x1 = (pos.x + hitboxOffset.x + size.x - 1) / tileSize;
	y = (pos.y + hitboxOffset.y + size.y - 1) / tileSize;

	for (int x = x0; x <= x1; x++)
	{
		if (map[(y + 1) * mapSize.x + x] == 3)
		{
			return true;
		}
	}
	return false;
}


void CollisionMap::setScene(Scene* scene) {
	this->scene = scene;
}