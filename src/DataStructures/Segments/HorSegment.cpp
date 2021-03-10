#include "HorSegment.h"
#include "Line.h"
#include "CircularArc.h"

// DEFAULT CONSTRUCTOR
HorSegment::HorSegment()
{
}

// OVERLOAD CONSTRUCTOR (Straight Line)
// ------------------------------------
//HorSegment::HorSegment(const glm::vec3& _p0, const glm::vec3& _p1)
HorSegment::HorSegment(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2)
{
	this->segment = new Line(_p1, _p2);
}
// OVERLOAD CONSTRUCTOR (Horizontal Curve)
// ---------------------------------------
//HorSegment::HorSegment(const glm::vec3& _p0, const glm::vec3& _p1, const glm::vec3& _p2)
HorSegment::HorSegment(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2, const CRAB::Vector4Df& _p3)
{
	this->segment = new CircularArc(_p1, _p2, _p3);
}
// DESTRUCTOR
// ----------
HorSegment::~HorSegment()
{
}