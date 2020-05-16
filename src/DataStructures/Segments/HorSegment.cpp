#include "HorSegment.h"
#include "Line.h"
#include "CircularArc.h"

// DEFAULT CONSTRUCTOR
HorSegment::HorSegment()
{
}

// OVERLOAD CONSTRUCTOR (Straight Line)
// ------------------------------------
HorSegment::HorSegment(const glm::vec3& _p0, const glm::vec3& _p1)
{
	this->segment = new Line(_p0, _p1);
}
// OVERLOAD CONSTRUCTOR (Horizontal Curve)
// ---------------------------------------
HorSegment::HorSegment(const glm::vec3& _p0, const glm::vec3& _p1, const glm::vec3& _p2)
{
	this->segment = new CircularArc(_p0, _p1, _p2);
}
// DESTRUCTOR
// ----------
HorSegment::~HorSegment()
{
}