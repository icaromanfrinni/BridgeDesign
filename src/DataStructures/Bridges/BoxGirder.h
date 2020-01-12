#pragma once
#include "Mesh.h"

class BoxGirder
{
public:
	std::vector<Mesh::Vertex> vertices;

	BoxGirder(const float width, const float length);
	~BoxGirder();
};

