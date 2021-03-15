#include "VerSegment.h"
#include "Line.h"
#include "ParabolicArc.h"

// DEFAULT CONSTRUCTOR
// -------------------
VerSegment::VerSegment()
{
}

// OVERLOAD CONSTRUCTOR (Straight Line)
// ------------------------------------
//VerSegment::VerSegment(const glm::vec3& _p0, const glm::vec3& _p1)
//{
//	this->segment = new Line(_p0, _p1);
//}
// OVERLOAD CONSTRUCTOR (Straight Line from CRAB::Vector)
// ------------------------------------------------------
VerSegment::VerSegment(const CRAB::Vector4Df& _p0, const CRAB::Vector4Df& _p1)
{
	this->segment = new Line(_p0, _p1);
}
// OVERLOAD CONSTRUCTOR (Parabolic Arc)
// ---------------------------------------
//VerSegment::VerSegment(const glm::vec3& _p0, const glm::vec3& _p1, const glm::vec3& _p2)
//{
//	this->segment = new ParabolicArc(_p0, _p1, _p2);
//}
// OVERLOAD CONSTRUCTOR (Parabolic Arc from CRAB::Vector)
// -----------------------------------------------------
VerSegment::VerSegment(const CRAB::Vector4Df& _p0, const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2)
{
	this->segment = new ParabolicArc(_p0, _p1, _p2);
}
// OVERLOAD CONSTRUCTOR (Parabolic Arc from endpoints)
// ---------------------------------------------------
VerSegment::VerSegment(const CRAB::Vector4Df& _p0, const CRAB::Vector4Df& _p2, const float& A)
{
	this->segment = new ParabolicArc(_p0, _p2, A);
}
// OVERLOAD CONSTRUCTOR (Parabolic Arc from midpoint)
// --------------------------------------------------
VerSegment::VerSegment(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _tan1, const CRAB::Vector4Df& _tan2, const float& L)
{
	this->segment = new ParabolicArc(_p1, _tan1, _tan2, L);
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
CRAB::Vector4Df VerSegment::getStartPoint4D() const
{
	return segment->getStartPoint4D();
}
CRAB::Vector4Df VerSegment::getMidPoint4D() const
{
	return segment->getMidPoint4D();
}
CRAB::Vector4Df VerSegment::getEndPoint4D() const
{
	return segment->getEndPoint4D();
}

// RETURN THE ELEVATION
// --------------------
float VerSegment::getY(const float& x) const
{
	return segment->getY(x);
}

// RETURN THE CLOSEST COLLISION DISTANCE
// -------------------------------------
CRAB::Vector4Df VerSegment::Collision(const CRAB::Ray& ray) const
{
	return segment->Collision(ray);
}

// RETURN TRUE IF THE POINT 'P' INTERSECT THE SEGMENT
// --------------------------------------------------
bool VerSegment::Contains(const CRAB::Vector4Df& p) const
{
	return segment->Contains(p);
}