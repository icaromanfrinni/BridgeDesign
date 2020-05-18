#include "Road.h"
#include "EulerOp.h"

// DEFAULT CONSTRUCTOR
// -------------------
Road::Road()
{
}

// OVERLOAD CONSTRUCTOR
// --------------------
Road::Road(const std::string& _name, const float& _width, const float& _speed, Alignment* _alignment)
	: name(_name), width(_width), speed(_speed), alignment(_alignment)
{
	std::cout << std::endl;
	std::cout << "\tNEW Road ................................. " << name << std::endl;
	// Stopping Sight Distance (S)
	// ---------------------------
	float d1 = 0.278 * speed * t;				// brake reaction distance
	float d2 = 0.039 * powf(speed, 2.0f) / a;	// braking distance on level
	S = round((d1 + d2) / 5) * 5;

	// Model
	update();
}

// DESTRUCTOR
// ----------
Road::~Road()
{
}

// UPDATE THE MODEL
void Road::update()
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
	CRAB::Vector4Df vUp = alignment->getNormalUp(t, this->speed);
	CRAB::Vector4Df vRight = cross(alignment->getTangent(t), vUp).to_unitary();

	// v0
	start_point = alignment->getPosition(0.0f);// .segments.front()->getStartPoint();
	EulerOp::mvfs(model, start_point);
	model.back()->name = "ROAD";
	model.back()->material = { 0.8f, 0.8f, 0.8f, 1.0f };
	// v1
	newVertex = model.back()->vertices.back()->point - (vRight * (this->width / 2.0f));
	EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);
	// v2
	newVertex = model.back()->vertices.back()->point - (vUp * 1.0f);
	EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
	// v3
	newVertex = model.back()->vertices.back()->point + (vRight * this->width);
	EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
	// v4
	newVertex = model.back()->vertices.back()->point + (vUp * 1.0f);
	EulerOp::mev(model.back()->halfEdges[4], NULL, 3, newVertex);
	// f1
	EulerOp::mef(model.back()->halfEdges[0], model.back()->halfEdges[7], 0);

	// SWEEP
	EulerOp::SWEEP(model.back()->faces.front(), alignment, speed);
}