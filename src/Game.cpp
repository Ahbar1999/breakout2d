#include "Game.h"
#include "ParticleGenerator.h"

SpriteRenderer* Renderer;

Game::Game(unsigned int Width, unsigned int Height): State(GAME_ACTIVE), Keys(), Width(Width), Height(Height)
{
	//Empty for now 
}

//Initial size of player
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
//Initial velocity of player
const float PLAYER_VELOCTIY(500.0f);
GameObject* Player;

//Initialize variables of Ball object
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float  BALL_RADIUS = 12.5f;
BallObject* Ball;

//Particles
ParticleGenerator* Particles;

void Game::Init()
{
	ResourceManager::LoadShader("vertex.shader", "fragment.shader", nullptr, "sprite");
	ResourceManager::LoadShader("vParticles.shader", "fParticles.shader", nullptr, "particle");
	//configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

	//Setting sprite shader uniform variables
	ResourceManager::getShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::getShader("sprite").SetMatrix4("projection", projection);

	//Setting Particle shader uniform variables
	ResourceManager::getShader("particle").Use().SetMatrix4("projection", projection);
	ResourceManager::getShader("particle").SetInteger("sprite", 0);
	//ResourceManager::getShader("particle").SetVector2f("offset", glm::vec2(0.5, 0));
	//ResourceManager::getShader("particle").SetVector4f("color", glm::vec4(1));

	Renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));

	ResourceManager::LoadTexture("../Assets/xqxgoblinmode.PNG", true, "demo");
	ResourceManager::LoadTexture("../Assets/background.jpg", false, "background");
	ResourceManager::LoadTexture("../Assets/block.png", false, "block");
	ResourceManager::LoadTexture("../Assets/block_solid.png", false, "block_solid");
	ResourceManager::LoadTexture("../Assets/paddle.png", true, "paddle");
	ResourceManager::LoadTexture("../Assets/awesomeface.png", true, "face");
	ResourceManager::LoadTexture("../Assets/particle.png", true, "particle");

	//load levels
	GameLevel one;
	one.Load("../Assets/Levels/Standard.lvl.txt", this->Width, this->Height/2);
	GameLevel two;
	two.Load("../Assets/Levels/two.lvl.txt", this->Width, this->Height / 2);
	GameLevel three;
	three.Load("../Assets/Levels/three.lvl.txt", this->Width, this->Height / 2);
	GameLevel four;
	four.Load("../Assets/Levels/four.lvl.txt", this->Width, this->Height / 2);

	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;

	glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexture("paddle"));
	
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::getTexture("face"));
	
	Particles = new ParticleGenerator(ResourceManager::getShader("particle"), ResourceManager::getTexture("particle"), 500);

}

void Game::Render() {
	if (this->State == GAME_ACTIVE) {
		//draw background
		Renderer->DrawSprite(ResourceManager::getTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
	}

	//draw level
	this->Levels[this->Level].Draw(*Renderer);
	//Draw player
	Player->Draw(*Renderer);
	//Draw particles
	Particles->Draw();
	//Draw ball, in that order
	Ball->Draw(*Renderer);
}

void Game::ProcessInput(float dt)
{
	if (this->State == GAME_ACTIVE)
	{
		float velocity = PLAYER_VELOCTIY * dt;
		//move paddle
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0.0f)
			{
				Player->Position.x -= velocity;
				if (Ball->Stuck)
					Ball->Position.x -= velocity;
			}

		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x < this->Width - Player->Size.x)
			{
				Player->Position.x += velocity;
				if (Ball->Stuck)
					Ball->Position.x += velocity;
			}
		}
		if (this->Keys[GLFW_KEY_SPACE])
			Ball->Stuck = false;
	}
}

bool Game::CheckCollision(GameObject& one, GameObject& two)
{
	//collision along x axis											//this is to check the second box hasnt crossed on the left side				
	bool CollisionX = (one.Position.x + one.Size.x >= two.Position.x) && (two.Position.x + two.Size.x >= one.Position.x);
	
	//collision along y axis
	bool CollisionY = (one.Position.y + one.Size.y >= two.Position.y) && (two.Position.y + two.Size.y >= one.Position.y);
	
	return CollisionX && CollisionY;

}

Collision Game::CheckCollision(BallObject& one, GameObject& two)
{
	//Ball's center
	glm::vec2 center(one.Position + one.Radius);

	//aabb's half width and half height(half-extents)
	float h = two.Size.y / 2;
	float w = two.Size.x / 2;
	glm::vec2 aabb_half_extents(w, h);
	//aabb center
	glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
	//vector between two centers(ball center and aabb center)
	glm::vec2 difference_vec = center - aabb_center;
	//clamped VECTOR(vector from a point on boundary to the center)
	glm::vec2 clamped = glm::clamp(difference_vec, -aabb_half_extents, aabb_half_extents);
	
	//closest POINT on aabb box to the circle
	glm::vec2 closest = clamped + aabb_center;

	glm::vec2 difference_vec_final = closest - center;

	if (glm::length(difference_vec_final) < one.Radius)
		return std::make_tuple(true, VectorDirection(difference_vec_final), difference_vec_final);
	else
		return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}


Direction Game::VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f), //up
		glm::vec2(1.0f, 0.0f), //right
		glm::vec2(0.0f, -1.0f), //down
		glm::vec2(-1.0f, 0.0f) //left
	};
	//dot product value for least angle of incidence with the direction
	float max = 0.0f;
	//Direction enum
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}

	return (Direction)best_match;
}


void Game::DoCollisions()
{
	for (GameObject& box : this->Levels[this->Level].Bricks)
	{
		if (!box.Destroyed)
		{
			Collision collision = CheckCollision(*Ball, box);	

			if (std::get<0>(collision)) //if collision is true 
			{
				if (!box.IsSolid)
				{
					box.Destroyed = true;
				}
				//Collision resolution
				Direction dir = std::get<1>(collision);
				glm::vec2 difference_vec = std::get<2>(collision);
				if (dir == LEFT || dir == RIGHT) // HORIZONTAL COLLISION
				{
					Ball->Velocity.x = -Ball->Velocity.x; // reverse the horizontal component of the velocity
					//relocate
					float penetration = Ball->Radius - std::abs(difference_vec.x); //radius - length of diff vector 
					if (dir == LEFT)
						Ball->Position.x += penetration; //move ball to right
					else
						Ball->Position.x -= penetration; //move ball to left
				}
				else //vertical collision
				{
					Ball->Velocity.y = -Ball->Velocity.y;

					float penetration = Ball->Radius - std::abs(difference_vec.y);
					if (dir == UP)
						Ball->Position -= penetration; //move the ball up (remember that Y axis is inverted in our coordinate system)
					else
						Ball->Position.y += penetration;

				}
			}
		}
	}
	//Collision between the paddle and the ball
	Collision result = CheckCollision(*Ball, *Player);
	if (!Ball->Stuck && std::get<0>(result))
	{
		//check wether it hit the board and change the horizontal velocity based on the distance from the center
		float centerBoard = Player->Position.x + Player->Size.x / 2.0f;
		float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
		float percentage = distance / (Player->Size.x / 2.0f);

		float strength = 2.0f;
		glm::vec2 oldVelocity = Ball->Velocity;
		Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		//Ball->Velocity.y = -Ball->Velocity.y;
		Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);
		Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
	}
}

void Game::Update(float dt)
{
	Ball->Move(dt, this->Width);
	this->DoCollisions();

	Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius/2.0f));

	if (Ball->Position.y >= this->Height)
	{
		this->ResetLevel();
		this->ResetPlayer();
	}
}

void Game::ResetLevel()
{
	if (this->Level == 0)
		this->Levels[0].Load("Assets/Levels/standard.lvl.txt", this->Width, this->Height / 2);
	else if (this->Level == 1)
		this->Levels[1].Load("Assets/Levels/two.lvl.txt", this->Width, this->Height / 2);
	else if (this->Level == 1)
		this->Levels[2].Load("Assets/Levels/three.lvl.txt", this->Width, this->Height / 2);
	else if (this->Level == 1)
		this->Levels[3].Load("Assets/Levels/four.lvl.txt", this->Width, this->Height / 2);

}

void Game::ResetPlayer()
{
	//reset ball player stats
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
	Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
}
