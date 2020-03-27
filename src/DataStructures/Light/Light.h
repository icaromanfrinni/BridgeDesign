#pragma once

#include <glad\glad.h>
#include <glm\glm.hpp>

class Light
{
public:
	glm::vec3 intensity;

	Light();
	Light(glm::vec3 _intensity);

	~Light();
};

