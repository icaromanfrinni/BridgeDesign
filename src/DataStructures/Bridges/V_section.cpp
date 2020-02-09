#include "V_section.h"
#include "EulerOp.h"

// default Constructor
V_section::V_section()
{
	// Input Parameters
	B = 10.8f;
	L = 35.0f;

	update();
}

// overload constructor
V_section::V_section(float _B, float _L, const CURVE::Bezier& _alignment)
{
	B = _B;
	L = _L;
	alignment = _alignment;

	update();
}

// destructor
V_section::~V_section()
{

}

void V_section::update()
{
	// Bridge Attributes
	float H = int((100.0f * L / 16.0f) / 5.0f) * 0.05f;
	float Lb = int((100.0f * B / 4.3f) / 5.0f) * 0.05f;
	float h = int((100.0f * Lb / 10.0f) / 5.0f) * 0.05f;
	float bw = int((100.0f * H / 5.0f) / 5.0f) * 0.05f;
	if (bw < 0.25f) bw = 0.25f;
	float tv = int((100.0f * (H - 2 * h) / 10.0f) / 5.0f) * 0.05f;
	if (tv < 0.10f) tv = 0.10f;
	float b = int((100.0f * (B - 2.0f * (Lb + INCLINATION_RATIO * (H - h - tv)))) / 5.0f) * 0.05f;
	float th = int((100.0f * (b - 2 * bw) / 5.0f) / 5.0f) * 0.05f;

	//Initialize
	CRAB::Vector4Df newVertex, next_position, start_point;
	float offset; // displacement
	float segment_L; // the length of the line segment
	float t = 0.0f; // [0; 1]

	// POLYGON FACE
	// ------------

	// Local axis
	CRAB::Vector4Df vRight = cross(alignment.tan(t), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
	CRAB::Vector4Df vUp = cross(vRight, alignment.tan(t)).to_unitary();

#pragma region TOP_LAYER
	// v0
	start_point = alignment.control_points[0];// -(vUp * TOP_LAYER);
	EulerOp::mvfs(model, start_point);
	model.back()->name = "TOP_LAYER";
	// v1
	newVertex = model.back()->vertices.back()->point - (vRight * (B / 2.0f - GUARD_RAIL)) - (vUp * (B / 2.0f - GUARD_RAIL) * SLOPE);
	EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);
	// v2
	newVertex = model.back()->vertices.back()->point + (vRight * (B - 2.0f * GUARD_RAIL));
	EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
	// f1
	EulerOp::mef(model.back()->halfEdges[0], model.back()->halfEdges[3], 0);

	// FIRST SWEEP
	t = 1.0f / alignment.segments;
	next_position = alignment.pos(t);
	segment_L = (next_position - alignment.control_points[0]).length();
	EulerOp::SWEEP(model.back()->faces.back(), alignment.tan(t), segment_L);
	start_point = next_position;

	// SWEEP
	for (int i = 1; i < alignment.segments; i++)
	{
		t += 1.0f / alignment.segments;
		next_position = alignment.pos(t);
		segment_L = (next_position - start_point).length();

		EulerOp::SWEEP(model.back()->faces[0], alignment.tan(t), segment_L);

		start_point = next_position;
	}
#pragma endregion TOP_LAYER

#pragma region DECK
	// OFFSET
	offset = (B / 2.0f - GUARD_RAIL) * SLOPE;
	// v0
	start_point = alignment.control_points[0] - (vUp * offset);
	EulerOp::mvfs(model, start_point);
	model.back()->name = "DECK";
	// v1
	newVertex = model.back()->vertices.back()->point - (vRight * (B / 2.0f - GUARD_RAIL));
	EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);
	// v2
	newVertex = model.back()->vertices.back()->point - (vUp * 0.20f);
	EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
	// v3
	newVertex = model.back()->vertices.back()->point - (vUp * (h + tv - 0.20f)) + (vRight * (Lb - GUARD_RAIL));
	EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
	// v4
	float hyp = sqrtf(powf(INCLINATION_RATIO * bw, 2.0f) + powf(bw, 2.0f));
	newVertex = model.back()->vertices.back()->point + (vRight * hyp);
	EulerOp::mev(model.back()->halfEdges[4], NULL, 3, newVertex);
	// v5
	newVertex = model.back()->vertices.back()->point + (vUp * tv) + (vRight * th);
	EulerOp::mev(model.back()->halfEdges[6], NULL, 4, newVertex);
	// v6
	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[5]->point - model.back()->vertices[0]->point, vUp);
	EulerOp::mev(model.back()->halfEdges[8], NULL, 5, newVertex);
	// v7
	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[4]->point - model.back()->vertices[0]->point, vUp);
	EulerOp::mev(model.back()->halfEdges[10], NULL, 6, newVertex);
	// v8
	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[3]->point - model.back()->vertices[0]->point, vUp);
	EulerOp::mev(model.back()->halfEdges[12], NULL, 7, newVertex);
	// v9
	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[2]->point - model.back()->vertices[0]->point, vUp);
	EulerOp::mev(model.back()->halfEdges[14], NULL, 8, newVertex);
	// v10
	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[1]->point - model.back()->vertices[0]->point, vUp);
	EulerOp::mev(model.back()->halfEdges[16], NULL, 9, newVertex);
	// f1
	EulerOp::mef(model.back()->halfEdges[0], model.back()->halfEdges[19], 0);

	// FIRST SWEEP
	// -----------
	t = 1.0f / alignment.segments;
	{	// UPDATE Local axis
		vRight = cross(alignment.tan(t), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
		vUp = cross(vRight, alignment.tan(t)).to_unitary();
	}
	next_position = alignment.pos(t) - (vUp * offset);
	segment_L = (next_position - start_point).length();
	EulerOp::SWEEP(model.back()->faces.back(), alignment.tan(t), segment_L);
	start_point = next_position;

	// SWEEP
	for (int i = 1; i < alignment.segments; i++)
	{
		t += 1.0f / alignment.segments;
		{	// UPDATE Local axis
			vRight = cross(alignment.tan(t), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
			vUp = cross(vRight, alignment.tan(t)).to_unitary();
		}
		next_position = alignment.pos(t) - (vUp * offset);
		segment_L = (next_position - start_point).length();
		EulerOp::SWEEP(model.back()->faces[0], alignment.tan(t), segment_L);
		start_point = next_position;
	}
#pragma endregion DECK

#pragma region U_SECTION
	// UPDATE Local axis
	t = 0.0f;
	vRight = cross(alignment.tan(t), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
	vUp = cross(vRight, alignment.tan(t)).to_unitary();
	// OFFSET
	offset = (B / 2.0f - GUARD_RAIL) * SLOPE + H;
	// v0
	start_point = alignment.control_points[0] - (vUp * offset);
	EulerOp::mvfs(model, start_point);
	model.back()->name = "U_SECTION";
	// v1
	newVertex = model.back()->vertices.back()->point + (vRight * (b / 2.0f));
	EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);
	// v2
	newVertex = model.back()->vertices.back()->point + (vRight * (B - b - 2 * Lb) / 2.0f) + (vUp * (H - h - tv));
	EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
	// v3
	newVertex = model.back()->vertices.back()->point - (vRight * hyp);
	EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
	// v4
	newVertex = model.back()->vertices.back()->point - (vUp * (H - 2 * (h + tv))) - (vRight * (INCLINATION_RATIO * ((H - 2 * (h + tv)))));
	EulerOp::mev(model.back()->halfEdges[4], NULL, 3, newVertex);
	// v5
	newVertex = model.back()->vertices.back()->point - (vUp * tv) - (vRight * th);
	EulerOp::mev(model.back()->halfEdges[6], NULL, 4, newVertex);
	// MIRROR
	// v6
	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[5]->point - model.back()->vertices[0]->point, vUp);
	EulerOp::mev(model.back()->halfEdges[8], NULL, 5, newVertex);
	// v7
	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[4]->point - model.back()->vertices[0]->point, vUp);
	EulerOp::mev(model.back()->halfEdges[10], NULL, 6, newVertex);
	// v8
	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[3]->point - model.back()->vertices[0]->point, vUp);
	EulerOp::mev(model.back()->halfEdges[12], NULL, 7, newVertex);
	// v9
	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[2]->point - model.back()->vertices[0]->point, vUp);
	EulerOp::mev(model.back()->halfEdges[14], NULL, 8, newVertex);
	// v10
	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[1]->point - model.back()->vertices[0]->point, vUp);
	EulerOp::mev(model.back()->halfEdges[16], NULL, 9, newVertex);
	// f1
	EulerOp::mef(model.back()->halfEdges[0], model.back()->halfEdges[19], 0);

	// FIRST SWEEP
	// -----------
	t = 1.0f / alignment.segments;
	{	// UPDATE Local axis
		vRight = cross(alignment.tan(t), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
		vUp = cross(vRight, alignment.tan(t)).to_unitary();
	}
	next_position = alignment.pos(t) - (vUp * offset);
	segment_L = (next_position - start_point).length();
	EulerOp::SWEEP(model.back()->faces.back(), alignment.tan(t), segment_L);
	start_point = next_position;

	// SWEEP
	for (int i = 1; i < alignment.segments; i++)
	{
		t += 1.0f / alignment.segments;
		{	// UPDATE Local axis
			vRight = cross(alignment.tan(t), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
			vUp = cross(vRight, alignment.tan(t)).to_unitary();
		}
		next_position = alignment.pos(t) - (vUp * offset);
		segment_L = (next_position - start_point).length();
		EulerOp::SWEEP(model.back()->faces[0], alignment.tan(t), segment_L);
		start_point = next_position;
	}
#pragma endregion U_SECTION
		
}