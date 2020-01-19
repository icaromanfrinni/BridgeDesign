#include "V_section.h"
#include "EulerOp.h"

// default Constructor
V_section::V_section()
{
	// Road Attributes
	start_point = { 0.0f, 0.0f, 0.0f, 1.0f };
	direction = { 0.0f, 0.0f, -1.0f, 0.0f };

	// Input Parameters
	B = 10.8f;
	L = 35.0f;

	update();
}

// overload constructor
V_section::V_section(float B, float L, const CRAB::Vector4Df& refPoint, const CRAB::Vector4Df& dir)
{

}

// destructor
V_section::~V_section()
{

}

void V_section::update()
{
	// Bridge Attributes
	H = int((100.0f * L / 16.0f) / 5.0f) * 0.05f;
	Lb = int((100.0f * B / 4.3f) / 5.0f) * 0.05f;
	h = int((100.0f * Lb / 10.0f) / 5.0f) * 0.05f;
	bw = int((100.0f * H / 5.0f) / 5.0f) * 0.05f;
	if (bw < 0.25f) bw = 0.25f;
	tv = int((100.0f * (H - 2 * h) / 10.0f) / 5.0f) * 0.05f;
	if (tv < 0.10f) tv = 0.10f;
	b = int((100.0f * (B - 2.0f * (Lb + INCLINATION_RATIO * (H - h - tv)))) / 5.0f) * 0.05f;
	th = int((100.0f * (b - 2 * bw) / 5.0f) / 5.0f) * 0.05f;

	// Local axis
	CRAB::Vector4Df vRight = cross(direction, { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
	CRAB::Vector4Df vUp = cross(vRight, direction).to_unitary();

	//Initialize New Vertex
	CRAB::Vector4Df newVertex;

	// POLYGON FACE (GL_TRIANGLE_FAN)
	// ------------------------------
	// TOP_LAYER
	// v0
	newVertex = start_point - (vUp * TOP_LAYER);
	EulerOp::mvfs(model, newVertex);
	model->name = "V_section";
	// v1
	newVertex = model->vertices.back()->point - (vRight * B / 2.0f) - (vUp * B / 2.0f * SLOPE);
	EulerOp::mev(model->faces[0]->hEdge, NULL, 0, newVertex);
	// v2
	newVertex = model->vertices.back()->point + (vRight * B);
	EulerOp::mev(model->halfEdges[0], NULL, 1, newVertex);
	// f1
	EulerOp::mef(model->halfEdges[0], model->halfEdges[3], 0);
	// "U" SECTION
	// v3
	newVertex = start_point - (vUp * TOP_LAYER);
	EulerOp::mvfs(model, newVertex);
	model->name = "V_section";
	// v1
	newVertex = model->vertices.back()->point - (vRight * B / 2.0f) - (vUp * B / 2.0f * SLOPE);
	EulerOp::mev(model->faces[0]->hEdge, NULL, 0, newVertex);
	// v2
	newVertex = model->vertices.back()->point + (vRight * B);
	EulerOp::mev(model->halfEdges[0], NULL, 1, newVertex);
	// f1
	EulerOp::mef(model->halfEdges[0], model->halfEdges[3], 0);

	// TRIANGULATED FACE (GL_TRIANGLES)
	// --------------------------------
	/*
	// FIRST CROSS SECTION
	// v0
	newVertex = start_point - (vUp * TOP_LAYER);
	EulerOp::mvfs(model, newVertex);
	model->name = "V_section";
	// v1
	newVertex = model->vertices.back()->point - (vRight * B / 2.0f) - (vUp * B / 2.0f * SLOPE);
	EulerOp::mev(model->faces[0]->hEdge, NULL, 0, newVertex);
	// v2, f0
	newVertex = model->vertices.back()->point + (vRight * (B - b) / 2.0f) - (vUp * h);
	EulerOp::mev(model->halfEdges[0], NULL, 1, newVertex);
	EulerOp::mef(model->halfEdges.back(), model->halfEdges[0], 0);
	// v3, f1
	newVertex = model->vertices[2]->point - (vUp * (H - h));
	EulerOp::mev(model->halfEdges[3], NULL, 1, newVertex);
	EulerOp::mef(model->halfEdges.back(), model->halfEdges[3], 1);
	// v4, f2
	newVertex = model->vertices[1]->point + (vRight * B / 2.0f) - (vUp * (H - h));
	EulerOp::mev(model->halfEdges[8], NULL, 3, newVertex);
	EulerOp::mef(model->halfEdges.back(), model->halfEdges[8], 2);
	// v5, f3
	newVertex = model->vertices[3]->point + (vRight * b);
	EulerOp::mev(model->halfEdges[11], NULL, 3, newVertex);
	EulerOp::mef(model->halfEdges.back(), model->halfEdges[11], 3);
	// v6, f4
	newVertex = model->vertices[2]->point + (vRight * b);
	EulerOp::mev(model->halfEdges[16], NULL, 5, newVertex);
	EulerOp::mef(model->halfEdges.back(), model->halfEdges[16], 4);
	// v7, f5
	newVertex = model->vertices[1]->point + (vRight * B);
	EulerOp::mev(model->halfEdges[19], NULL, 5, newVertex);
	EulerOp::mef(model->halfEdges.back(), model->halfEdges[19], 5);
	// f6
	EulerOp::mef(model->halfEdges[24], model->halfEdges[12], 6);
	// f7
	EulerOp::mef(model->halfEdges[24], model->halfEdges[4], 7);
	// f8
	EulerOp::mef(model->halfEdges[23], model->halfEdges[28], 8);

	// EXTRUDE
	// v8, f9
	newVertex = model->vertices[0]->point + (direction * L);
	EulerOp::mev(model->halfEdges[1], NULL, 0, newVertex);
	EulerOp::mef(model->halfEdges.back(), model->halfEdges[1], 9);
	// v9, f10
	newVertex = model->vertices[1]->point + (direction * L);
	EulerOp::mev(model->halfEdges.back()->opp, NULL, 8, newVertex);
	EulerOp::mef(model->halfEdges.back(), model->halfEdges[34], 10);
	// v10, f11
	newVertex = model->vertices[2]->point + (direction * L);
	EulerOp::mev(model->halfEdges.back()->opp, NULL, 9, newVertex);
	EulerOp::mef(model->halfEdges[33], model->halfEdges.back(), 11);
	*/
	
}