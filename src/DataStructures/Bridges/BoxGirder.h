#pragma once
#include "Mesh.h"

class BoxGirder
{
public:
	std::vector<Vertex> vertices;

	BoxGirder(const float width, const float length);
	~BoxGirder();
};

