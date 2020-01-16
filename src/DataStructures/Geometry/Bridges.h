#pragma once
#include "HalfEdge.h"
#include "EulerOp.h"

// Default bridge values
const float SLOPE = 0.02f;
const float REV = 0.07f;
const float GUARDRAIL = 0.40f;
const float HM = 0.10f;

namespace BRIDGES
{
	void BoxGirder(std::vector<HED::solid*>& model, float B, float L, const CRAB::Vector4Df& refPoint, const CRAB::Vector4Df& dir)
	{
		// Local axis
		CRAB::Vector4Df vRight = cross(dir, { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
		CRAB::Vector4Df vUp = cross(vRight, dir).to_unitary();

		//Initialize New Vertex
		CRAB::Vector4Df newVertex;

		EulerOp::mvfs(model, refPoint);
		model.back()->name = "Box_Girder";
		// -------------------
		newVertex = model.back()->vertices.back()->point + (vRight * B);
		EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);
		// -------------------
		newVertex = model.back()->vertices.back()->point + (vUp * L);
		EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
		// -------------------
		EulerOp::mef(model.back()->halfEdges.back(), model.back()->halfEdges[0], 0);
	}
}

