#pragma once
#ifndef GAME_H
#define GAME_H

//Using external libs(glfw and glad) with vcpkg package manager
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include <vector>
#include "GameLevel.h"
#include "BallObject.h"
#include <tuple>


//You know the drill
enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

class Game
{
public:
	//game state
	GameState State;
	bool Keys[1024];
	unsigned int Width, Height;

	//constructor/destructors
	Game(unsigned int width, unsigned int height);
	//~Game();

	//Initialize game state, load all textures/shaders/levels
	void Init();
	
	//game loop
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();
	
	//collision info 
	bool CheckCollision(GameObject& one, GameObject& two); //AABB-AABB collision detection procedure
	//Overloaded method for checking collisions with round object using modified AABB-AABB collision detection procedure	
	Collision CheckCollision(BallObject& one, GameObject& two);
	void DoCollisions();
	Direction VectorDirection(glm::vec2 target);


	//Level info
	std::vector<GameLevel> Levels; //all the levels
	unsigned int Level;	//current active level
	
	//Reset Functions
	void ResetLevel();
	void ResetPlayer();
};

#endif // !GAME_H
