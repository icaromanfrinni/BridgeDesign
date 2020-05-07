#include "Bezier.h"

// DEFAULT CONSTRUCTOR
// -------------------
Bezier::Bezier()
{
	this->points.push_back(CRAB::Vector4Df{ 10.0f, 0.0f, 2.0f, 1.0f });
	this->points.push_back(CRAB::Vector4Df{ 10.0f, 10.0f, 2.0f, 1.0f });
	this->points.push_back(CRAB::Vector4Df{ 0.0f, 10.0f, 2.0f, 1.0f });
}

// DESTRUCTOR
// ----------
Bezier::~Bezier()
{
}

// BASIS FUNCTIONS
// ---------------
int Bezier::binomialCoefficient(const int& n, const int& i) const
{
	if (i == 0 || i == n)
		return 1;
	return binomialCoefficient(n - 1, i - 1) + binomialCoefficient(n - 1, i);
}
float Bezier::BernsteinPolynomial(const int& n, const int& i, const float& t) const
{
	if (i < 0 || i > n)
		return 0;
	int C = binomialCoefficient(n, i);
	return C * powf(t, i) * powf(1 - t, n - i);
}

// CALCULATES THE DERIVATIVE FOR A CURVE OF ORDER 'n'
// --------------------------------------------------
CRAB::Vector4Df Bezier::deriv(float t) const
{
	CRAB::Vector4Df d = { 0.0f, 0.0f, 0.0f, 0.0f };
	int n = points.size() - 1;
	if (n >= 1) // at least 2 control points
		for (int i = 0; i < n; i++) {
			d += (points[i + 1] - points[i]) * n * BernsteinPolynomial(n - 1, i, t);
		}
	return d;
}
CRAB::Vector4Df Bezier::deriv2(float t) const
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
CRAB::Vector4Df Bezier::getPosition(const float& t) const
{
	CRAB::Vector4Df position = { 0.0f, 0.0f, 0.0f, 1.0f };
	int n = points.size() - 1;
	for (int i = 0; i < points.size(); i++) {
		position += points[i] * BernsteinPolynomial(n, i, t);
	}
	return position;
}
// RETURNS THE CURVE TANGENT
// -------------------------
CRAB::Vector4Df Bezier::getTangent(const float& t) const
{
	return deriv(t).to_unitary();
}
// RETURNS THE CURVE NORMAL
// ------------------------
CRAB::Vector4Df Bezier::getNormal(const float& t) const
{
	CRAB::Vector4Df d1 = deriv(t);
	CRAB::Vector4Df d2 = deriv2(t);

	CRAB::Vector4Df k = d2 - d1 * (dot(d2, d1) / d1.lengthsq());
	return k.to_unitary();
}
// RETURNS THE CURVE NORMAL UP (Yaw vector)
// ----------------------------------------
CRAB::Vector4Df Bezier::getNormalUp(const float& t) const
{
	CRAB::Vector4Df vUp = { 0.0f, 1.0f, 0.0f, 0.0f };
	CRAB::Vector4Df vAux = CRAB::cross(vUp, getTangent(t)).to_unitary();
	CRAB::Vector4Df n = CRAB::cross(getTangent(t), vAux).to_unitary();
	return n;
}
// RETURNS THE CURVE BINORMAL
// --------------------------
CRAB::Vector4Df Bezier::getBinormal(const float& t) const
{
	return CRAB::cross(getTangent(t), getNormal(t)).to_unitary();
}
// RETURNS THE CURVATURE
// ---------------------
float Bezier::getCurvature(const float& t) const
{
	float k = cross(deriv(t), deriv2(t)).length() / powf(deriv(t).length(), 3.0f);
	return k;
}
// RETURNS THE RADIUS OF CURVATURE
// -------------------------------
float Bezier::getRadius(const float& t) const
{
	float r = 1.0f / getCurvature(t);
	return r;
}
// CLOCKWISE CHECK
// ---------------
bool Bezier::isClockwise(const float& t) const
{
	CRAB::Vector4Df vup = cross(this->getNormal(t), this->getTangent(t)).to_unitary();
	float dotP = dot(vup, this->getNormalUp(t));
	if (dotP > 0.0f)
		return true;
	else return false;
}