#include "Road.h"
#include "EulerOp.h"

// DEFAULT CONSTRUCTOR
// -------------------
Road::Road()
{
}

// OVERLOAD CONSTRUCTOR
// --------------------
Road::Road(const std::string& _name, const float& _width, const float& _speed, Alignment* _alignment, Vehicle* _vehicle)
	: name(_name), width(_width), speed(_speed), alignment(_alignment), vehicle(_vehicle)
{
	// Model
	//update();

	std::cout << "\n\tNEW Road ................................. " << name << std::endl;
}

// DESTRUCTOR
// ----------
Road::~Road()
{
}

// ROAD DESIGN
// -----------
float Road::StoppingSightDistance()
{
	float d1 = 0.278 * this->speed * reactionTime;			// brake reaction distance
	float d2 = 0.039 * powf(this->speed, 2.0f) / decelRate;	// braking distance on level
	return round((d1 + d2) / 5) * 5;
}

// UPDATE THE MODEL
//void Road::update()
//{
//	// Initialize
//	solids.clear();
//	CRAB::Vector4Df newVertex, next_position, start_point;
//	float offset; // displacement
//	float segment_L; // the length of the line segment
//	float t = 0.0f; // [0, 1]
//
//	// POLYGON FACE (GL_TRIANGLE_FAN)
//	// ------------------------------
//
//	// Local axis
//	CRAB::Vector4Df vUp = alignment->getNormalUp(t/*, this->speed*/);
//	CRAB::Vector4Df vRight = cross(alignment->getTangent(t), vUp).to_unitary();
//
//	// v0
//	start_point = alignment->getPosition(0.0f);// .segments.front()->getStartPoint();
//	EulerOp::mvfs(solids, start_point);
//	solids.back()->name = "ROAD";
//	solids.back()->material = { 0.8f, 0.8f, 0.8f, 1.0f };
//	// v1
//	newVertex = solids.back()->vertices.back()->point - (vRight * (this->width / 2.0f));
//	EulerOp::mev(solids.back()->faces[0]->hEdge, NULL, 0, newVertex);
//	// v2
//	newVertex = solids.back()->vertices.back()->point - (vUp * 1.0f);
//	EulerOp::mev(solids.back()->halfEdges[0], NULL, 1, newVertex);
//	// v3
//	newVertex = solids.back()->vertices.back()->point + (vRight * this->width);
//	EulerOp::mev(solids.back()->halfEdges[2], NULL, 2, newVertex);
//	// v4
//	newVertex = solids.back()->vertices.back()->point + (vUp * 1.0f);
//	EulerOp::mev(solids.back()->halfEdges[4], NULL, 3, newVertex);
//	// f1
//	EulerOp::mef(solids.back()->halfEdges[0], model.back()->halfEdges[7], 0);
//
//	// SWEEP
//	EulerOp::SWEEP(solids.back()->faces.front(), alignment);
//}