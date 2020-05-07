#include "BoxGirder.h"
#include "EulerOp.h"

// DEFAULT CONSTRUCTOR
BoxGirder::BoxGirder()
{
}

// OVERLOAD CONSTRUCTOR
BoxGirder::BoxGirder(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance)
	: Bridge(_name, _road, cross_station, vertical_clearance, horizontal_clearance)
{
	// Box-Girder Bridge Attributes
	Lb = int((100.0f * B / 4.3f) / 5.0f) * 0.05f;
	h = int((100.0f * Lb / 10.0f) / 5.0f) * 0.05f;
	bw = int((100.0f * H / 5.0f) / 5.0f) * 0.05f;
	if (bw < 0.25f) bw = 0.25f;
	tv = int((100.0f * (H - 2 * h) / 10.0f) / 5.0f) * 0.05f;
	if (tv < 0.10f) tv = 0.10f;
	b = int((100.0f * (B - 2.0f * (Lb + INCLINATION_RATIO * (H - h - tv)))) / 5.0f) * 0.05f;
	th = int((100.0f * (b - 2 * bw) / 5.0f) / 5.0f) * 0.05f;

	// PIERS
	// -----

	/*float b_Pier = b;
	float h_Pier = 0.4f * b_Pier;
	float total_length = (this->alignment.back()->getEndPoint() - this->alignment.front()->getStartPoint()).length();
	int nPiers = total_length / this->mainSpan;
	float g = (this->vertical_clearance + this->H) / (total_length / 2.0f);
	CRAB::Vector4Df CS = this->road->alignment.back()->getPosition(this->cross_station / this->road->alignment.back()->getLength());*/

	/*CRAB::Vector4Df base = this->alignment.front()->getStartPoint() + road->alignment.back()->getTan(0) * mainSpan;
	for (int i = 1; i < nPiers; i++)
	{
		Pier P;
		P.b = b_Pier;
		P.h = h_Pier;
		P.base = base;
		base += road->alignment.back()->getTan(0) * mainSpan;
		P.L = (this->vertical_clearance + this->H) - (P.base - CS).length() * g;
		piers.push_back(P);
	}*/

	// Model
	update();
}

// DESTRUCTOR
BoxGirder::~BoxGirder()
{
}

// UPDATE THE MODEL
void BoxGirder::update()
{
	// Initialize
	model.clear();
	CRAB::Vector4Df newVertex, next_position, start_point;
	float offset; // displacement
	float segment_L; // the length of the line segment
	float t = 0.0f; // [0, 1]

	// POLYGON FACE (GL_TRIANGLE_FAN)
	// ------------------------------

	// Local axis
	//CRAB::Vector4Df vRight = cross(alignment.getTangent(t), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
	//CRAB::Vector4Df vUp = cross(vRight, alignment.getTangent(t)).to_unitary();
	CRAB::Vector4Df vUp = alignment->getNormalUp(t);
	CRAB::Vector4Df vRight = cross(alignment->getTangent(t), vUp).to_unitary();

#pragma region TOP_LAYER
	// v0
	start_point = alignment->getPosition(0.0f);// -(vUp * TOP_LAYER);
	EulerOp::mvfs(model, start_point);
	model.back()->name = "TOP_LAYER";
	model.back()->material = { 0.1f, 0.1f, 0.1f, 1.0f };
	// v1
	newVertex = model.back()->vertices.back()->point - (vRight * (B / 2.0f - GUARD_RAIL)) - (vUp * (B / 2.0f - GUARD_RAIL) * SLOPE);
	EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);
	// v2
	newVertex = model.back()->vertices.back()->point - (vUp * TOP_LAYER);
	EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
	// v3
	newVertex = model.back()->vertices.back()->point + (vRight * (B - 2.0f * GUARD_RAIL));
	EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
	// v4
	newVertex = model.back()->vertices.back()->point + (vUp * TOP_LAYER);
	EulerOp::mev(model.back()->halfEdges[4], NULL, 3, newVertex);
	// f1
	EulerOp::mef(model.back()->halfEdges[0], model.back()->halfEdges[7], 0);

	// SWEEP
	EulerOp::SWEEP(model.back()->faces.front(), alignment);
#pragma endregion TOP_LAYER

#pragma region DECK
	// OFFSET
	offset = (B / 2.0f - GUARD_RAIL) * SLOPE + TOP_LAYER;
	// v0
	start_point = alignment->getPosition(0.0f) - (vUp * offset);
	EulerOp::mvfs(model, start_point);
	model.back()->name = "DECK";
	model.back()->material = { 0.8f, 0.8f, 0.8f, 1.0f };
	// v1
	newVertex = model.back()->vertices.back()->point - (vRight * (B / 2.0f - GUARD_RAIL));
	EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);

	// Left Guard-Rail
	{
		// V2
		newVertex = model.back()->vertices.back()->point + (vUp * 0.15f);
		EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
		// V3
		newVertex = model.back()->vertices.back()->point + (vUp * 0.25f) - (vRight * 0.175f);
		EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
		// V4
		newVertex = model.back()->vertices.back()->point + (vUp * 0.47f) - (vRight * 0.05f);
		EulerOp::mev(model.back()->halfEdges[4], NULL, 3, newVertex);
		// V5
		newVertex = model.back()->vertices.back()->point - (vRight * 0.175f);
		EulerOp::mev(model.back()->halfEdges[6], NULL, 4, newVertex);
		// V6
		newVertex = model.back()->vertices.back()->point - (vUp * 1.12f);
		EulerOp::mev(model.back()->halfEdges[8], NULL, 5, newVertex);
		// V7
		newVertex = model.back()->vertices.back()->point - (vUp * 0.05f) + (vRight * 0.05f);
		EulerOp::mev(model.back()->halfEdges[10], NULL, 6, newVertex);
		// V8
		newVertex = model.back()->vertices.back()->point + (vUp * 0.10f) + (vRight * 0.35f);
		EulerOp::mev(model.back()->halfEdges[12], NULL, 7, newVertex);
	}
	// v9
	newVertex = model.back()->vertices.back()->point - (vUp * (h + tv - 0.20f)) + (vRight * (Lb - GUARD_RAIL));
	EulerOp::mev(model.back()->halfEdges[14], NULL, 8, newVertex);
	// v10
	float hyp = sqrtf(powf(INCLINATION_RATIO * bw, 2.0f) + powf(bw, 2.0f)); // Hypotenuse = top width of beam
	newVertex = model.back()->vertices.back()->point + (vRight * hyp);
	EulerOp::mev(model.back()->halfEdges[16], NULL, 9, newVertex);
	// v11
	newVertex = model.back()->vertices.back()->point + (vUp * tv) + (vRight * th);
	EulerOp::mev(model.back()->halfEdges[18], NULL, 10, newVertex);

	// MIRROR
	{
		// v12
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[11]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[20], NULL, 11, newVertex);
		// v13
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[10]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[22], NULL, 12, newVertex);
		// v14
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[9]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[24], NULL, 13, newVertex);
		// v15
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[8]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[26], NULL, 14, newVertex);
		// v16
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[7]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[28], NULL, 15, newVertex);
		// v17
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[6]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[30], NULL, 16, newVertex);
		// v18
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[5]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[32], NULL, 17, newVertex);
		// v19
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[4]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[34], NULL, 18, newVertex);
		// v20
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[3]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[36], NULL, 19, newVertex);
		// v21
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[2]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[38], NULL, 20, newVertex);
		// v22
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[1]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[40], NULL, 21, newVertex);
	}

	// f1
	EulerOp::mef(model.back()->halfEdges[0], model.back()->halfEdges[43], 0);

	// SWEEP
	EulerOp::SWEEP(model.back()->faces.front(), alignment);

#pragma endregion DECK

#pragma region U_SECTION
	// UPDATE Local axis
	t = 0.0f;
	//vRight = cross(alignment.getTangent(t), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
	//vUp = cross(vRight, alignment.getTangent(t)).to_unitary();
	vUp = alignment->getNormalUp(t);
	vRight = cross(alignment->getTangent(t), vUp).to_unitary();
	// OFFSET
	offset = (B / 2.0f - GUARD_RAIL) * SLOPE + H + TOP_LAYER;
	// v0
	start_point = alignment->getPosition(0.0f) - (vUp * offset);
	EulerOp::mvfs(model, start_point);
	model.back()->name = "U_SECTION";
	model.back()->material = { 0.8f, 0.8f, 0.8f, 1.0f };
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

	// SWEEP
	EulerOp::SWEEP(model.back()->faces.front(), alignment);

#pragma endregion U_SECTION

#pragma region ROADS
	//vRight = cross(road->alignment.segments.front()->getTan(0.0f), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
	//vUp = cross(vRight, road->alignment.segments.front()->getTan(0.0f)).to_unitary();
	////for (int i = 0; i < piers.size(); i++)
	////{
	//	// v0
	//	start_point = road->alignment.segments.front()->getStartPoint();
	//	EulerOp::mvfs(model, start_point);
	//	model.back()->name = "ROAD";
	//	model.back()->material = { 0.8f, 0.8f, 0.8f, 1.0f };
	//	// v1
	//	newVertex = model.back()->vertices.back()->point - (vRight * (road->width / 2.0f));
	//	EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);
	//	// v2
	//	newVertex = model.back()->vertices.back()->point - (vUp * 1.0f);
	//	EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
	//	// v3
	//	newVertex = model.back()->vertices.back()->point + (vRight * road->width);
	//	EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
	//	// v4
	//	newVertex = model.back()->vertices.back()->point + (vUp * 1.0f);
	//	EulerOp::mev(model.back()->halfEdges[4], NULL, 3, newVertex);
	//	// f1
	//	EulerOp::mef(model.back()->halfEdges[0], model.back()->halfEdges[7], 0);

	//	// SWEEP
	//	for (int i = 0; i < road->alignment.segments.size(); i++)
	//	{
	//		if (i == 0) // First segment
	//		{
	//			// First Sweep
	//			t = 1.0f / DIVIDER;
	//			{	// UPDATE Local axis
	//				vRight = cross(road->alignment.segments[i]->getTan(t), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
	//				vUp = cross(vRight, road->alignment.segments[i]->getTan(t)).to_unitary();
	//			}
	//			next_position = road->alignment.segments[i]->getPosition(t) - (vUp * offset);
	//			segment_L = (next_position - start_point).length();
	//			EulerOp::SWEEP(model.back()->faces.back(), road->alignment.segments[i]->getTan(t), segment_L);
	//			start_point = next_position;

	//			for (int j = 1; j < DIVIDER; j++)
	//			{
	//				t += 1.0f / DIVIDER;
	//				{	// UPDATE Local axis
	//					vRight = cross(road->alignment.segments[i]->getTan(t), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
	//					vUp = cross(vRight, road->alignment.segments[i]->getTan(t)).to_unitary();
	//				}
	//				next_position = road->alignment.segments[i]->getPosition(t) - (vUp * offset);
	//				segment_L = (next_position - start_point).length();
	//				EulerOp::SWEEP(model.back()->faces[0], road->alignment.segments[i]->getTan(t), segment_L);
	//				start_point = next_position;
	//			}
	//		}
	//		else // Others segments
	//		{
	//			t = 0.0f;
	//			for (int j = 0; j < DIVIDER; j++)
	//			{
	//				t += 1.0f / DIVIDER;
	//				{	// UPDATE Local axis
	//					vRight = cross(road->alignment.segments[i]->getTan(t), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
	//					vUp = cross(vRight, road->alignment.segments[i]->getTan(t)).to_unitary();
	//				}
	//				next_position = road->alignment.segments[i]->getPosition(t) - (vUp * offset);
	//				segment_L = (next_position - start_point).length();
	//				EulerOp::SWEEP(model.back()->faces[0], road->alignment.segments[i]->getTan(t), segment_L);
	//				start_point = next_position;
	//			}
	//		}
	//	}
	////}
#pragma endregion 
}

