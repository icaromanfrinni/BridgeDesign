#include "BoxGirder.h"

BoxGirder::BoxGirder(const float width, const float length)
{
	float height = 2.5f;
	Mesh::Vertex vertex;

	vertex.Position = { 0.0f, 0.0f, 0.0f };
	vertex.Normal = { 0.0f, 0.0f, -1.0f };
	vertices.push_back(vertex);
	vertex.Position = { width, 0.0f, 0.0f };
	vertex.Normal = { 0.0f, 0.0f, -1.0f };
	vertices.push_back(vertex);
	vertex.Position = { width, height, 0.0f };
	vertex.Normal = { 0.0f, 0.0f, -1.0f };
	vertices.push_back(vertex);

	vertex.Position = { width, height, 0.0f };
	vertex.Normal = { 0.0f, 0.0f, -1.0f };
	vertices.push_back(vertex);
	vertex.Position = { 0.0f, height, 0.0f };
	vertex.Normal = { 0.0f, 0.0f, -1.0f };
	vertices.push_back(vertex);
	vertex.Position = { 0.0f, 0.0f, 0.0f };
	vertex.Normal = { 0.0f, 0.0f, -1.0f };
	vertices.push_back(vertex);

	vertex.Position = { 0.0f, height, 0.0f };
	vertex.Normal = { 0.0f, 1.0f, 0.0f };
	vertices.push_back(vertex);
	vertex.Position = { width, height, 0.0f };
	vertex.Normal = { 0.0f, 1.0f, 0.0f };
	vertices.push_back(vertex);
	vertex.Position = { width, height, -length };
	vertex.Normal = { 0.0f, 1.0f, 0.0f };
	vertices.push_back(vertex);
}

BoxGirder::~BoxGirder()
{
}