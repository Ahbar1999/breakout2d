#pragma once
#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H

#include <vector>
#include "Shader.h"
#include "GameObject.h"

//Represents a single particle and its state
struct Particle
{
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	float Life;

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f)
	{	}
};

class ParticleGenerator
{
/// <summary>
/// Public members of this class include:
/// 	The Constructor : As usual to initialize the variables
/// 	The Update Method: To update the the current state of the particles
/// 	The Draw Method : To Draw/Render the particles on the screen
/// </summary>
public:
	//Constructor
	ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);
	//update  all particles 
	void Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	//render  all particles 
	void Draw();

private:
	//state
	std::vector<Particle> Particles;	//A vector to hold all of our particles
	unsigned int amount;	//Amount of particles

	//render state
	Shader shader;
	Texture2D texture;
	
	/// <summary>
	/// We need this globally decalared handler for a VAO.. 
	/// ..so we can access the mesh/vertex data in different method
	/// </summary>
	unsigned int VAO;

	void init();	//In this init() method we initialize all the vertex buffer data 

	unsigned int firstUnusedParticle(); //returns the first particle index thats unused e.g Life <= 0.0f or 0 if no particle is currently active
	//respawns particle
	void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif // !PARTICLE_GENERATOR_H