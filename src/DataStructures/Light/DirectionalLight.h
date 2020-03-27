#pragma once
#include "Light.h"

class DirectionalLight :
	public Light
{
public:
	glm::vec3 direction;

	DirectionalLight();
	DirectionalLight(glm::vec3 _intensity, glm::vec3 _direction);
	~DirectionalLight();

	void UseLight(GLint intensityLocation, GLint directionLocation) const;
};

