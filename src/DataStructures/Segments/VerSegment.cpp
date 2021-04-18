#include "VerSegment.h"

// DEFAULT CONSTRUCTOR
// -------------------
VerSegment::VerSegment()
{
}

// OVERLOAD CONSTRUCTOR (Straight Line)
// ------------------------------------
VerSegment::VerSegment(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p3)
	: p1(_p1), p3(_p3)
{
	this->p2 = (this->p1 + this->p3) / 2.0f;

	this->tan1 = (this->p2 - this->p1).to_unitary();
	this->tan2 = (this->p3 - this->p2).to_unitary();

	this->g1 = this->tan1.y / this->tan1.x;
	this->g2 = this->tan2.y / this->tan2.x;
}
// OVERLOAD CONSTRUCTOR (Parabolic Arc)
// ------------------------------------
VerSegment::VerSegment(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2, const CRAB::Vector4Df& _p3)
	: p1(_p1), p2(_p2), p3(_p3)
{
	this->tan1 = (this->p2 - this->p1).to_unitary();
	this->tan2 = (this->p3 - this->p2).to_unitary();

	this->g1 = this->tan1.y / this->tan1.x;
	this->g2 = this->tan2.y / this->tan2.x;
}
// OVERLOAD CONSTRUCTOR (Parabolic Arc from endpoints)
// ---------------------------------------------------
VerSegment::VerSegment(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p3, const float& A)
	: p1(_p1), p3(_p3)
{
	float L = this->p3.x - this->p1.x;
	float g3 = (this->p3.y - this->p1.y) / L;
	this->g1 = (A / 200.0f) + g3;
	this->g2 = -(A / 200.0f) + g3;

	this->p2 = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->p2.x = this->p1.x + (L / 2.0f);
	this->p2.y = this->p1.y + this->g1 * (L / 2.0f);

	this->tan1 = (this->p2 - this->p1).to_unitary();
	this->tan2 = (this->p3 - this->p2).to_unitary();
}
// OVERLOAD CONSTRUCTOR (Parabolic Arc from midpoint)
// --------------------------------------------------
VerSegment::VerSegment(const CRAB::Vector4Df& _p2, const CRAB::Vector4Df& _tan1, const CRAB::Vector4Df& _tan2, const float& L)
	: p2(_p2), tan1(_tan1), tan2(_tan2)
{
	this->g1 = this->tan1.y / this->tan1.x;
	this->g2 = this->tan2.y / this->tan2.x;

	this->p1 = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->p1.x = this->p2.x - (L / 2.0f);
	this->p1.y = this->p2.y - this->g1 * (L / 2.0f);

	this->p3 = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->p3.x = this->p2.x + (L / 2.0f);
	this->p3.y = this->p2.y + this->g2 * (L / 2.0f);
}

// DESTRUCTOR
// ----------
VerSegment::~VerSegment()
{
}

// RETURN (glm::vec3)
// ------------------
glm::vec3 VerSegment::getStartPoint3D() const {
	return glm::vec3(this->p1.x, this->p1.y, this->p1.z);
}
glm::vec3 VerSegment::getMidPoint3D() const {
	return glm::vec3(this->p2.x, this->p2.y, this->p2.z);
}
glm::vec3 VerSegment::getEndPoint3D() const {
	return glm::vec3(this->p3.x, this->p3.y, this->p3.z);
}

// RETURN (CRAB::Vector4Df)
// ------------------------
CRAB::Vector4Df VerSegment::getStartPoint4D() const {
	return this->p1;
}
CRAB::Vector4Df VerSegment::getMidPoint4D() const {
	return this->p2;
}
CRAB::Vector4Df VerSegment::getEndPoint4D() const {
	return this->p3;
}

// RETURN THE ELEVATION
// --------------------
float VerSegment::getY(const float& x) const
{
	float dX = x - this->p1.x;
	float A = 0.5f * (this->g2 - this->g1) / (this->p3.x - this->p1.x);
	float y = A * powf(dX, 2.0f) + this->g1 * dX + this->p1.y;
	return y;
}

// RETURN THE INCLINATION OF A RAMP
// --------------------------------
float VerSegment::getG1() const {
	return this->g1;
}
float VerSegment::getG2() const {
	return this->g2;
}