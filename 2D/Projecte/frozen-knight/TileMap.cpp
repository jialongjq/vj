#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include "Scene.h"
#include "MediaPlayer.h"

using namespace std;

TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	this->minCoords = minCoords;
	loadLevel(levelFile);
	prepareArrays(program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	
	nActivable = 0;
	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			fin.get(tile);
			if (tile == ' ')
				map[j * mapSize.x + i] = 0;
			else if (tile >= '0' && tile <= '9') {
				map[j * mapSize.x + i] = tile - int('0');
				if (map[i + j * mapSize.x] == 1 || map[i + j * mapSize.x] == 3)
					nActivable += 1;
			}
			else
				map[j * mapSize.x + i] = 10 + tile - int('A');
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close(); 
	return true;
}

void TileMap::prepareArrays(ShaderProgram &program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	
	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());

	indices = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if (tile != 0)
			{
				indices[j * mapSize.x + i] = vertices.size();
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);

				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
			else
				indices[j * mapSize.x + i] = -1;
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

void TileMap::activateTile(glm::ivec2 &pos, const glm::ivec2 &size, const glm::ivec2& hitboxOffset)
{
	int x0 = (pos.x + hitboxOffset.x) / tileSize;
	int x1 = (pos.x + hitboxOffset.x + size.x -1) / tileSize;
	int y = (pos.y + hitboxOffset.y + size.y) / tileSize;

	for (int x = x0; x <= x1; ++x) {
		if (indices[x + y * mapSize.x] != -1) {
			bool changed = false;
			if (map[x + y * mapSize.x] == 1 || map[x + y * mapSize.x] == 3) {
				map[x + y * mapSize.x] += 1;
				changed = true;
				nActivable -= 1;
				if (nActivable == 0) {
					key->show();
				}
			}
			if (changed) {
				MediaPlayer* mediaPlayer = MediaPlayer::getInstance();
				mediaPlayer->playTileSound();
				scene->increaseScore(10);

				int index = indices[x + y * mapSize.x];

				int tile = map[y * mapSize.x + x];

				glm::vec2 posTile, texCoordTile[2];
				posTile = glm::vec2(minCoords.x + x * tileSize, minCoords.y + y * tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;

				// First triangle
				vertices[index + 2] = (texCoordTile[0].x); vertices[index + 3] = (texCoordTile[0].y);
				vertices[index + 6] = (texCoordTile[1].x); vertices[index + 7] = (texCoordTile[0].y);
				vertices[index + 10] = (texCoordTile[1].x); vertices[index + 11] = (texCoordTile[1].y);

				// Second triangle
				vertices[index + 14] = (texCoordTile[0].x); vertices[index + 15] = (texCoordTile[0].y);
				vertices[index + 18] = (texCoordTile[1].x); vertices[index + 19] = (texCoordTile[1].y);
				vertices[index + 22] = (texCoordTile[0].x); vertices[index + 23] = (texCoordTile[1].y);
			}
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
}

void TileMap::setKey(Key* key) {
	this->key = key;
}


void TileMap::setScene(Scene* scene) {
	this->scene = scene;
}