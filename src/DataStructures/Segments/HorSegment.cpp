#include "HorSegment.h"

// DEFAULT CONSTRUCTOR
HorSegment::HorSegment()
{
}

// OVERLOAD CONSTRUCTOR (Straight Line)
// ------------------------------------
HorSegment::HorSegment(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p3)
	: p1(_p1), p3(_p3)
{
	this->p2 = (this->p1 + this->p3) / 2.0f;
}
// OVERLOAD CONSTRUCTOR (Circular Arc)
// -----------------------------------
HorSegment::HorSegment(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2, const CRAB::Vector4Df& _p3)
	: p1(_p1), p2(_p2), p3(_p3)
{
}
// DESTRUCTOR
// ----------
HorSegment::~HorSegment()
{
}

// RETURN (glm::vec3)
// ------------------
glm::vec3 HorSegment::getStartPoint3D() const {
	return glm::vec3(p1.x, p1.y, p1.z);
}
glm::vec3 HorSegment::getMidPoint3D() const {
	return glm::vec3(p2.x, p2.y, p2.z);
}
glm::vec3 HorSegment::getEndPoint3D() const {
	return glm::vec3(p3.x, p3.y, p3.z);
}
// RETURN (CRAB::Vector4Df)
// ------------------------
CRAB::Vector4Df HorSegment::getStartPoint4D() const {
	return this->p1;
}
CRAB::Vector4Df HorSegment::getMidPoint4D() const {
	return this->p2;
}
CRAB::Vector4Df HorSegment::getEndPoint4D() const {
	return this->p3;
}

// RETURN THE WEIGHT OF THE CONTROL POINT
// --------------------------------------
float HorSegment::midPointWeight() const
{
	CRAB::Vector4Df p1p2 = (this->p2 - this->p1).to_unitary();
	CRAB::Vector4Df p1p3 = (this->p3 - this->p1).to_unitary();
	return CRAB::dot(p1p2, p1p3);
}