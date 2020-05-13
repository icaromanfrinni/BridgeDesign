#include "VerSegment.h"
#include "Line.h"
#include "CircularArc.h"

// DEFAULT CONSTRUCTOR
// -------------------
VerSegment::VerSegment()
{
}

// OVERLOAD CONSTRUCTOR (Straight Line)
// ------------------------------------
VerSegment::VerSegment(const glm::vec3& _p0, const glm::vec3& _p1)
{
	this->segment = new Line(_p0, _p1);
}
// OVERLOAD CONSTRUCTOR (Vertical Curve)
// ---------------------------------------
VerSegment::VerSegment(const glm::vec3& _p0, const glm::vec3& _p1, const glm::vec3& _p2)
{
	this->segment = new CircularArc(_p0, _p1, _p2);
}

// DESTRUCTOR
// ----------
VerSegment::~VerSegment()
{
}

// RETURN (glm::vec3)
// ------------------
glm::vec3 VerSegment::getStartPoint() const
{
	return segment->getStartPoint();
}
glm::vec3 VerSegment::getMidPoint() const
{
	return segment->getMidPoint();
}
glm::vec3 VerSegment::getEndPoint() const
{
	return segment->getEndPoint();
}

// RETURN (CRAB::Vector4Df)
// ------------------------
CRAB::Vector4Df VerSegment::getStart4DPoint() const
{
	return segment->getStart4DPoint();
}
CRAB::Vector4Df VerSegment::getMid4DPoint() const
{
	return segment->getMid4DPoint();
}
CRAB::Vector4Df VerSegment::getEnd4DPoint() const
{
	return segment->getEnd4DPoint();
}

// RETURN THE ELEVATION
// --------------------
float VerSegment::getY(const float& x) const
{
	return segment->getY(x);
}