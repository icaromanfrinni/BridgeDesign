#pragma once

#include <string>

class Texture
{
private:
	//unsigned int Texture_ID;
	std::string File_Path;
	unsigned char* data;
	int Width, Height, m_BPP;
public:
	unsigned int Texture_ID;
	std::string type;
	Texture(const std::string& path, const std::string& typeName);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return Width; }
	inline int GetHeight() const { return Height; }
};