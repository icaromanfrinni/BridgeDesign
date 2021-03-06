#include "Material.h"

// DEFAULT CONSTRUCTOR
Material::Material()
{
	name = "default";
    ka = { 0.6f, 0.6f, 0.6f };
    kd = { 0.7f, 0.7f, 0.7f };
    ks = { 0.9f, 0.9f, 0.9f };
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