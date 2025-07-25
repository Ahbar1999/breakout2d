#pragma once
#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <iostream>
#include "GameObject.h"
#include <vector>

class GameLevel
{
public:

	//level state
	std::vector<GameObject> Bricks;
	//constructor 
	GameLevel() {}
	//loads the level from a file
	void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
	//render level
	void Draw(SpriteRenderer& Renderer);
	//check if the level is completed (all non solid tiles are destroyed)
	bool IsCompleted();

private:
	//initialize level from tile data
	void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};


#endif // !GAMELEVEL_H