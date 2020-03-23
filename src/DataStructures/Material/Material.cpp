#include "Material.h"

// DEFAULT CONSTRUCTOR
Material::Material()
{
	name = "default";
    ka = { 0.0f, 0.0f, 0.0f };
    kd = { 0.1f, 0.1f, 0.1f };
    ks = { 0.2f, 0.2f, 0.2f };
    shininess = 32.0f;
}


Material::Material(const std::string& _name, const glm::vec3& _ka, const glm::vec3& _kd, const glm::vec3& _ks, const float& _shininess) 
    : name(_name), ka(_ka), kd(_kd), ks(_ks), shininess(_shininess)
{
}

Material::Material(const std::vector<Texture*> _textures) : textures(_textures)
{
    hasTexture = true;
    shininess = 32.0f;
}

// DESTRUCTOR
Material::~Material()
{
}