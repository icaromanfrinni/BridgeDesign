#include "Material.h"

// DEFAULT CONSTRUCTOR
Material::Material()
{
	name = "default";
    ka = { 0.4f, 0.4f, 0.4f };
    kd = { 0.6f, 0.6f, 0.6f };
    ks = { 0.7f, 0.7f, 0.7f };
    shininess = 32.0f;

    hasTexture = false;
    
    textures.push_back(new Texture("textures/concrete_diffuse.png", "diffuse"));
    textures.push_back(new Texture("textures/concrete_specular.jpg", "specular"));
    textures.push_back(new Texture("textures/concrete_normal.jpg", "normal"));

    // ASPHALT
    /*textures.push_back(new Texture("textures/Asphalt_DIFFUSE.jpg", "diffuse"));
    textures.push_back(new Texture("textures/Asphalt_SPECULAR.jpg", "specular"));
    textures.push_back(new Texture("textures/Asphalt_NORMAL.jpg", "normal"));*/
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