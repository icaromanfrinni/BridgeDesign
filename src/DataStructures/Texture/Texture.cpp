#include "Texture.h"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

Texture::Texture(const std::string& path, const std::string& typeName) : Texture_ID(0), type(typeName), File_Path(path), data(nullptr), Width(0), Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(path.c_str(), &Width, &Height, &m_BPP, 4);

	glGenTextures(1, &Texture_ID);
	glBindTexture(GL_TEXTURE_2D, Texture_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR/*_MIPMAP_LINEAR*/);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
	}
	else std::cout << "Failed to load texture" << std::endl;		
}

Texture::~Texture()
{
	glDeleteTextures(1, &Texture_ID);
}

// Use Texture
void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, Texture_ID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}