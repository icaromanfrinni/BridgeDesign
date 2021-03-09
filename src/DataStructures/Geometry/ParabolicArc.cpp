#include "ParabolicArc.h"

// DEFAULT CONSTRUCTOR
// -------------------
ParabolicArc::ParabolicArc()
{
}
// OVERLOAD CONSTRUCTOR
// --------------------
ParabolicArc::ParabolicArc(const glm::vec3 &_p1, const glm::vec3 &_p2, const glm::vec3 &_p3)
	: p1(_p1), p2(_p2), p3(_p3)
{
	this->Setup();
}
// OVERLOAD CONSTRUCTOR (from CRAB::Vector4Df)
// -------------------------------------------
ParabolicArc::ParabolicArc(const CRAB::Vector4Df &_p1, const CRAB::Vector4Df &_p2, const CRAB::Vector4Df &_p3)
{
	this->p1 = glm::vec3(_p1.x, _p1.y, _p1.z);
	this->p2 = glm::vec3(_p2.x, _p2.y, _p2.z);
	this->p3 = glm::vec3(_p3.x, _p3.y, _p3.z);
	this->Setup();
}
// DESTRUCTOR
// ----------
ParabolicArc::~ParabolicArc()
{
}
// SETUP
// -----
void ParabolicArc::Setup()
{
	this->tan1 = glm::normalize(this->p2 - this->p1);
	this->tan2 = glm::normalize(this->p3 - this->p2);
	glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	
	this->g1 = tanf(acosf(glm::dot(xAxis, this->tan1)));
	if (glm::cross(xAxis, this->tan1).z < 0.0f)
		this->g1 *= -1.0f;

	this->g2 = tanf(acosf(glm::dot(xAxis, this->tan2)));
	if (glm::cross(xAxis, this->tan2).z < 0.0f)
		this->g2 *= -1.0f;
}

// RETURN (glm::vec3)
// ------------------
glm::vec3 ParabolicArc::getStartPoint() const
{
	return this->p1;
}
glm::vec3 ParabolicArc::getMidPoint() const
{
	return this->p2;
}
glm::vec3 ParabolicArc::getEndPoint() const
{
	return this->p3;
}

// RETURN (CRAB::Vector4Df)
// ------------------------
CRAB::Vector4Df ParabolicArc::getStart4DPoint() const
{
	return { this->p1.x, this->p1.y, this->p1.z, 1.0f };
}
CRAB::Vector4Df ParabolicArc::getMid4DPoint() const
{
	return { this->p2.x, this->p2.y, this->p2.z, 1.0f };
}
CRAB::Vector4Df ParabolicArc::getEnd4DPoint() const
{
	return { this->p3.x, this->p3.y, this->p3.z, 1.0f };
}

// RETURNS THE SEGMENT LENGTH
// --------------------------
float ParabolicArc::getLength() const
{
	float AC = acosf(glm::dot(this->tan1, this->tan2));
	float R = glm::length(this->p2 - this->p1) / tanf(AC / 2.0f);
	return AC * R;
}

// RETURN THE ELEVATION
// --------------------
float ParabolicArc::getY(const float& x) const
{
	float AC = acosf(glm::dot(this->tan1, this->tan2));
	float R = glm::length(this->p2 - this->p1) / tanf(AC / 2.0f);
	glm::vec3 n = glm::normalize(glm::cross(this->tan2, this->tan1));
	glm::vec3 r = glm::normalize(glm::cross(this->tan1, n));
	glm::vec3 center = this->p1 + (r * R);

	glm::vec3 z = { 0.0f, 0.0f, 1.0f };
	int a = 1; // crest
	if (glm::dot(n, z) < 0)
		a = -1; // sag

	float y = a * sqrtf(powf(R, 2.0f) - powf(x - center.x, 2.0f)) + center.y;
	return y;
}

// RETURN THE WEIGHT OF THE CONTROL POINT
// --------------------------------------
float ParabolicArc::midPointWeight() const
{
	return 1.0f;
}

// RETURN THE CLOSEST COLLISION DISTANCE OF A RAY AND THE SEGMENT
// --------------------------------------------------------------
CRAB::Vector4Df ParabolicArc::Collision(const CRAB::Ray& ray) const
{
	CRAB::Vector4Df Up = { 0.0f, 1.0f, 0.0f, 0.0f };

	// FIRST TANGENT
	// -------------
	// tangent vector
	CRAB::Vector4Df tan4D1 = { this->tan1.x, this->tan1.y, this->tan1.z, 0.0f };
	// normal vector
	CRAB::Vector4Df Right = CRAB::cross(tan4D1, Up);
	CRAB::Vector4Df normal = CRAB::cross(Right, tan4D1);
	// "t" distance
	float t1 = CRAB::dot(this->getStart4DPoint() - ray.origin, normal) / CRAB::dot(ray.direction, normal);

	// SECOND TANGENT
	// --------------
	// tangent vector
	CRAB::Vector4Df tan4D2 = { this->tan2.x, this->tan2.y, this->tan2.z, 0.0f };
	// normal vector
	Right = CRAB::cross(tan4D2, Up);
	normal = CRAB::cross(Right, tan4D2);
	// "t" distance
	float t2 = CRAB::dot(this->getEnd4DPoint() - ray.origin, normal) / CRAB::dot(ray.direction, normal);

	// Closest collision Point
	// -----------------------
	if (t1 < t2)
		return ray.origin + ray.direction * t1;
	else return ray.origin + ray.direction * t2;
}

// RETURN TRUE IF THE POINT 'P' INTERSECT THE SEGMENT
// --------------------------------------------------
bool ParabolicArc::Contains(const CRAB::Vector4Df& p) const
{
	CRAB::Vector4Df e1 = this->getMid4DPoint() - this->getStart4DPoint();
	CRAB::Vector4Df e2 = this->getEnd4DPoint() - this->getMid4DPoint();

	if ((p - this->getStart4DPoint()).length() <= e1.length() &&
		(p - this->getMid4DPoint()).length() <= e1.length())
		return true;
	else if ((p - this->getMid4DPoint()).length() <= e2.length() &&
		(p - this->getEnd4DPoint()).length() <= e2.length())
		return true;
	else return false;
}