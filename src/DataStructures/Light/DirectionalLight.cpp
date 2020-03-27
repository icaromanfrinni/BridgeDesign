#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
}

DirectionalLight::DirectionalLight(glm::vec3 _intensity, glm::vec3 _direction) : Light(_intensity), direction(_direction)
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::UseLight(GLint intensityLocation, GLint directionLocation) const
{
	glUniform3f(intensityLocation, intensity.x, intensity.y, intensity.z);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}


