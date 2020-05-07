#include "Line.h"

// DEFAULT CONSTRUCTOR
// -------------------
Line::Line()
{
}
// OVERLOAD CONSTRUCTOR (from CRAB::Vector4Df)
// -------------------------------------------
Line::Line(const glm::vec3 & _p0, const glm::vec3 & _p1)
	: p0(_p0), p1(_p1)
{
}
// DESTRUCTOR
// ----------
Line::~Line()
{
}

// RETURN
// ------
glm::vec3 Line::getStartPoint() const
{
	return p0;
}
glm::vec3 Line::getMidPoint() const
{
	return (p0 + p1) / 2.0f;
}
glm::vec3 Line::getEndPoint() const
{
	return p1;
}
float Line::getLength() const
{
	return glm::distance(p0, p1);
}
float Line::getY(const float& x) const
{
	float m = (p1.y - p0.y) / (p1.x - p0.x);
	float y = m * (x - p0.x) + p0.y;
	return y;
}

// RETURN THE WEIGHT OF THE CONTROL POINT
// --------------------------------------
float Line::midPointWeight() const
{
	return 1.0f;
}