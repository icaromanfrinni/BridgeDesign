#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "Texture.h"

class Material
{
public:
	std::string name;
	glm::vec3 ka;	// ambient
	glm::vec3 kd;	// diffuse
	glm::vec3 ks;	// specular
	float shininess;

	bool hasTexture = false;

	std::vector<Texture*> textures;

	// DEFAULT CONSTRUCTOR
	Material();
	// OVERLOAD CONSTRUCTOR
	Material(const std::string& _name, const glm::vec3& _ka, const glm::vec3& _kd, const glm::vec3& _ks, const float& _shininess);
	Material(const std::vector<Texture*> _textures);

	// DESTRUCTOR
	~Material();
};