#include "Alignment.h"

// DEFAULT CONSTRUCTOR
// -------------------
Alignment::Alignment()
{

}
// DESTRUCTOR
// ----------
Alignment::~Alignment()
{

}

int Alignment::binomialCoefficient(const int& n, const int& i) const
{
	if (i == 0 || i == n)
		return 1;
	return binomialCoefficient(n - 1, i - 1) + binomialCoefficient(n - 1, i);
}
float Alignment::BernsteinPolynomial(const int& n, const int& i, const float& t) const
{
	if (i < 0 || i > n)
		return 0;
	int C = binomialCoefficient(n, i);
	return C * powf(t, i) * powf(1 - t, n - i);
}

// CALCULATES THE DERIVATIVE FOR A CURVE OF ORDER 'n'
CRAB::Vector4Df Alignment::deriv(float t) const
{
	CRAB::Vector4Df d = { 0.0f, 0.0f, 0.0f, 0.0f };
	int n = points.size() - 1;
	if (n >= 1) // at least 2 control points
		for (int i = 0; i < n; i++) {
			d += (points[i + 1] - points[i]) * n * BernsteinPolynomial(n - 1, i, t);
		}
	return d;
}
CRAB::Vector4Df Alignment::deriv2(float t) const
{
	CRAB::Vector4Df d = { 0.0f, 0.0f, 0.0f, 0.0f };
	int n = points.size() - 1;
	if (n >= 2) // at least 3 control points
		for (int i = 0; i < (n - 1); i++) {
			d += (points[i + 2] - (points[i + 1] * 2.0f) + points[i]) * n * (n - 1) * BernsteinPolynomial(n - 2, i, t);
		}
	return d;
}

// RETURNS A POINT ON THE CURVE
// ----------------------------
CRAB::Vector4Df Alignment::getPosition(const float& t) const
{
	CRAB::Vector4Df position = { 0.0f, 0.0f, 0.0f, 1.0f };
	int n = points.size() - 1;
	for (int i = 0; i < points.size(); i++)
	{
		position += points[i] * BernsteinPolynomial(n, i, t);
	}
	return position;
}
// RETURNS THE CURVE TANGENT
// -------------------------
CRAB::Vector4Df Alignment::getTan(const float& t) const
{
	return deriv(t).to_unitary();
}
// RETURNS THE CURVE NORMAL
// ------------------------
CRAB::Vector4Df Alignment::getNormal(const float& t) const
{
	CRAB::Vector4Df k = deriv2(t) - deriv(t) * (dot(deriv2(t), deriv(t)) / deriv(t).lengthsq());
	return k.to_unitary();
}
// RETURNS THE CURVE NORMAL UP
// ---------------------------
CRAB::Vector4Df Alignment::getNormalUp(const float& t) const
{
	CRAB::Vector4Df vUp = { 0.0f, 1.0f, 0.0f, 0.0f };
	CRAB::Vector4Df vAux = CRAB::cross(vUp, getTan(t)).to_unitary();
	CRAB::Vector4Df n = CRAB::cross(getTan(t), vAux).to_unitary();
	return n;
}
// RETURNS THE CURVE BINORMAL
// --------------------------
CRAB::Vector4Df Alignment::getBinormal(float t) const
{
	return CRAB::cross(getTan(t), getNormal(t)).to_unitary();
}
// RETURNS THE CURVATURE
// ---------------------
float Alignment::getCurvature(float t) const
{
	float k = cross(deriv(t), deriv2(t)).length() / powf(deriv(t).length(), 3.0f);
	return k;
}
// RETURNS THE RADIUS OF CURVATURE
// -------------------------------
float Alignment::getRadius(float t) const
{
	float r = 1.0f / getCurvature(t);
	return r;
}
// RETURNS THE CURVE LENGTH
// ------------------------
float Alignment::getLength() const
{
	float length = 0.0f;
	// TODO
	return length;
}

// Return Point from Station
// -------------------------
CRAB::Vector4Df Alignment::getPointFromStation(float dist)
{
	CRAB::Vector4Df vPxz = this->getPosition(1.0f) - this->getPosition(0.0f);
	vPxz.y = 0.0f;
	float t = dist / vPxz.length();
	if (t <= 1.0f)
		return this->getPosition(t);
	else dist -= vPxz.length();

	return getPosition(t);
}

// Return Station from Point
// -------------------------
float Alignment::getStationFromPoint(CRAB::Vector4Df p)
{
	CRAB::Vector4Df v = p - this->getPosition(0.0f);
	v.y = 0.0f;
	return v.length();
}

// RETURN the closest collision distance of a ray and the segment
// --------------------------------------------------------------
CRAB::Vector4Df Alignment::Collision(const Ray& ray) const
{
	CRAB::Vector4Df Up = { 0.0f, 1.0f, 0.0f, 0.0f };

	// FIRST TANGENT
	// -------------

	// normal vector
	CRAB::Vector4Df Right = CRAB::cross(this->getTan(0.0f), Up);
	CRAB::Vector4Df normal = CRAB::cross(Right, this->getTan(0.0f));

	// "t" distance
	float t1 = CRAB::dot(this->getPosition(0.0f) - ray.origin, normal) / CRAB::dot(ray.direction, normal);

	// SECOND TANGENT
	// --------------

	// normal vector
	Right = CRAB::cross(this->getTan(1.0f), Up);
	normal = CRAB::cross(Right, this->getTan(1.0f));

	// "t" distance
	float t2 = CRAB::dot(this->getPosition(1.0f) - ray.origin, normal) / CRAB::dot(ray.direction, normal);

	// Closest collision Point
	// -----------------------

	if (t1 < t2)
		return ray.origin + ray.direction * t1;
	else return ray.origin + ray.direction * t2;
}
// RETURN true if the point P intersect the segment
// ------------------------------------------------
bool Alignment::Contains(const CRAB::Vector4Df& p) const
{
	/*CRAB::Vector4Df e1 = this->getMidPoint() - this->getStartPoint();
	CRAB::Vector4Df e2 = this->getEndPoint() - this->getMidPoint();

	if ((p - this->getStartPoint()).length() <= e1.length() &&
		(p - this->getMidPoint()).length() <= e1.length())
		return true;
	else if ((p - this->getMidPoint()).length() <= e2.length() &&
		(p - this->getEndPoint()).length() <= e2.length())
		return true;
	else return false;*/
	
	return false;
}