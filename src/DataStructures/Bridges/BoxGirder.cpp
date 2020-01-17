#include "BoxGirder.h"
#include "EulerOp.h"

// default Constructor
BoxGirder::BoxGirder()
{
	// Road Attributes
	start_point = { 0.0f, 0.0f, 0.0f, 1.0f };
	direction = { 0.0f, 0.0f, -1.0f, 0.0f };

	// Input Parameters
	B = 10.8f;
	L = 35.0f;

	update();
}

// destructor
BoxGirder::~BoxGirder()
{

}

void BoxGirder::update()
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

	// FIRST CROSS SECTION
	// -------------------
	EulerOp::mvfs(model, start_point);
	model->name = "Box_Girder";
	// Left Slope
	// ----------
	newVertex = start_point - (vRight * B / 2.0f) - (vUp * (B / 2.0f - GUARD_RAIL) * SLOPE);
	EulerOp::mev(model->faces[0]->hEdge, NULL, 0, newVertex);
	// Right Slope
	// --------------
	newVertex = start_point + (vRight * B / 2.0f) - (vUp * (B / 2.0f - GUARD_RAIL) * SLOPE);
	EulerOp::mev(model->halfEdges[0], NULL, 1, newVertex);
	// MEF
	// ---
	EulerOp::mef(model->halfEdges.back(), model->halfEdges[0], 0);
}