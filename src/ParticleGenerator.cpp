#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount)
	: shader(shader), texture(texture), amount(amount)
{
	this->init();
}

void ParticleGenerator::Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset)
{
	//add new particles
	for (unsigned int i = 0; i < newParticles; ++i)
	{
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->Particles[unusedParticle], object, offset);
	}

	//update all particles 
	for(unsigned int i = 0; i < this->amount; ++i)
	{
		Particle& p = this->Particles[i];
		p.Life -= dt; //reduce the particle's life
		if (p.Life > 0.0f)
		{
			//this particle is alive thus update
			p.Position -= p.Velocity * dt; //move
			p.Color -= dt * 2.5f; //and update the colors
		}
	}
}

void ParticleGenerator::Draw()
{
	//use additive blending to give it a glow effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->shader.Use();
	for(const Particle& particle: this->Particles)
	{ 
		if (particle.Life > 0)
		{
			this->shader.SetVector2f("offset", particle.Position);
			this->shader.SetVector4f("color", particle.Color);
			this->texture.Bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0); // unbind
		}
	}
	//reset to default blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void ParticleGenerator::init()
{
	//set up mesh and attributes properties
	unsigned int VBO;
	float particle_quad[] =
	{
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->VAO);	//Initialize the VAO we cerated as one fo one of our private member variables
	
	//Fill up the VAO with our mesh data
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	
	//FILL MESH BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	//SET MESH ATTRIBUTES
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindVertexArray(0);


	//create this->amount particles 
	for (unsigned int i = 0; i < this->amount; ++i)
	{
		this->Particles.push_back(Particle());

	}
}


//stores the index of the last particle used (for quick access to next dead particle)
unsigned int lastUsedParticle = 0;
//I dont understand how does this work
unsigned int ParticleGenerator::firstUnusedParticle()
{
	//first search from last used particle, this will usually return almost instantly
	for(unsigned int i = lastUsedParticle; i < this->amount; ++i)
	{
		if (this->Particles[i].Life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	//otherwise, do a linear search
	for (unsigned int i = 0; i < lastUsedParticle; ++i)
	{
		if (this->Particles[i].Life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}
	//all particles are taken, override the first one (note that if it repeatedly hits the case, more particles should be reserved)
	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset)
{
	float random = ((rand() % 100) - 50) / 10.0f;
	float rColor = 0.5f + ((rand() % 100) / 100.0f);
	particle.Position = object.Position + random + offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = object.Velocity * 0.1f;
}








