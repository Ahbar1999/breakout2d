#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"

class SpriteRenderer
{
public:
	SpriteRenderer(const Shader& shader);
	//~SpriteRenderer();

	void DrawSprite(const Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0, 10.0),
		float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:

	Shader shader;
	unsigned int quadVAO;

	void initRenderData();
};

