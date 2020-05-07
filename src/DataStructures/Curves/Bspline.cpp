#include "Bspline.h"

#include <iostream>

// DEFAULT CONSTRUCTOR
// -------------------
Bspline::Bspline()
{
	// control points
	this->points.push_back(CRAB::Vector4Df{ -5.0f, 0.0f, 0.0f, 1.0f });
	this->points.push_back(CRAB::Vector4Df{ 0.0f, 5.0f, 0.0f, 1.0f });
	this->points.push_back(CRAB::Vector4Df{ 5.0f, 0.0f, 0.0f, 1.0f });

	// degree
	this->p = this->points.size() - 1;

	// knot vector
	int m = points.size() + this->p;
	int n = points.size() - 1;
	for (int i = 0; i <= m; i++)
	{
		if (i <= p)
			T.push_back(0.0f);
		else if (i > n)
			T.push_back(1.0f);
		else
		{
			float u = float(i - p) / float(n - p + 1);
			T.push_back(u);
		}
	}
}

// DESTRUCTOR
// ----------
Bspline::~Bspline()
{
}

// BASIS FUNCTIONS
// ---------------
float Bspline::N(const int& i, const int& p, const float& t) const
{
	if (p == 0)
	{
		if (t >= this->T[i] && t < this->T[i + 1])
			return 1.0f;
		else if (t == this->T[i + 1] && t == this->T.back())
			return 1.0f;
		else return 0.0f;
	}

	float left = this->N(i, p - 1, t) * (t - this->T[i]) / (this->T[i + p] - this->T[i]);
	if (isnan(left)) left = 0.0f;

	float right = this->N(i + 1, p - 1, t) * (this->T[i + p + 1] - t) / (this->T[i + p + 1] - this->T[i + 1]);
	if (isnan(right)) right = 0.0f;

	return left + right;
}
float Bspline::dN(const int& i, const int& p, const float& t) const
{
	float left = this->N(i, p - 1, t) * p / (this->T[i + p] - this->T[i]);
	if (isnan(left)) left = 0.0f;

	float right = this->N(i + 1, p - 1, t) * p / (this->T[i + p + 1] - this->T[i + 1]);
	if (isnan(right)) right = 0.0f;

	return left - right;
}
float Bspline::dN2(const int& i, const int& p, const float& t) const
{
	float left = this->dN(i, p - 1, t) * p / (this->T[i + p] - this->T[i]);
	if (isnan(left)) left = 0.0f;

	float right = this->dN(i + 1, p - 1, t) * p / (this->T[i + p + 1] - this->T[i + 1]);
	if (isnan(right)) right = 0.0f;

	return left - right;
}

// DERIVATIVES
// -----------
CRAB::Vector4Df Bspline::deriv(const float& t) const
{
	CRAB::Vector4Df deriv = { 0.0f, 0.0f, 0.0f, 0.0f };
	int span = this->FindSpan(t);

	for (int i = 0; i <= this->p; i++)
	{
		int index = span - this->p + i;
		deriv += points[index] * this->dN(index, this->p, t);
	}

	return deriv;
}
CRAB::Vector4Df Bspline::deriv2(const float& t) const
{
	CRAB::Vector4Df deriv2 = { 0.0f, 0.0f, 0.0f, 0.0f };
	int span = this->FindSpan(t);

	for (int i = 0; i <= this->p; i++)
	{
		int index = span - this->p + i;
		deriv2 += points[index] * this->dN2(index, this->p, t);
	}
	
	return deriv2;
}

// FIND THE ith KNOT SPAN
// ----------------------
int Bspline::FindSpan(const float& t) const
{
	int n = points.size() - 1;
	if (t == T[n + 1])
		return n;

	int low = this->p;
	int high = n + 1;
	int mid = (low + high) / 2;

	while (t < T[mid] || t >= T[mid + 1])
	{
		if (t < T[mid])
			high = mid;
		else low = mid;
		
		mid = (low + high) / 2;
	}

	return mid;
}

// RETURNS A POINT ON THE CURVE
// ----------------------------
CRAB::Vector4Df Bspline::getPosition(const float& t) const
{
	CRAB::Vector4Df position = { 0.0f, 0.0f, 0.0f, 1.0f };
	int span = this->FindSpan(t);

	for (int i = 0; i <= this->p; i++)
	{
		position += points[span - this->p + i] * this->N(span - this->p + i, this->p, t);
	}
	return position;
}

// RETURNS THE CURVE TANGENT
// -------------------------
CRAB::Vector4Df Bspline::getTangent(const float& t) const
{
	return this->deriv(t).to_unitary();
}

// RETURNS THE CURVE NORMAL
// ------------------------
CRAB::Vector4Df Bspline::getNormal(const float& t) const
{
	CRAB::Vector4Df d1 = this->deriv(t);
	CRAB::Vector4Df d2 = this->deriv2(t);
	CRAB::Vector4Df n = d2 - d1 * (dot(d2, d1) / d1.lengthsq());
	return n.to_unitary();
}

// RETURNS THE CURVE NORMAL UP (Yaw vector)
// ----------------------------------------
CRAB::Vector4Df Bspline::getNormalUp(const float& t) const
{
	CRAB::Vector4Df tan = this->getTangent(t);
	CRAB::Vector4Df vUp = { 0.0f, 1.0f, 0.0f, 0.0f };
	CRAB::Vector4Df vAux = CRAB::cross(vUp, tan).to_unitary();
	CRAB::Vector4Df n = CRAB::cross(tan, vAux).to_unitary();
	return n;
}

// RETURNS THE CURVE BINORMAL
// --------------------------
CRAB::Vector4Df Bspline::getBinormal(const float& t) const
{
	return CRAB::cross(getTangent(t), getNormal(t)).to_unitary();
}

// RETURNS THE CURVATURE
// ---------------------
float Bspline::getCurvature(const float& t) const
{
	CRAB::Vector4Df deriv = this->deriv(t);
	CRAB::Vector4Df deriv2 = this->deriv2(t);
	float k = cross(deriv, deriv2).length() / powf(deriv.length(), 3.0f);

	return k;
}

// RETURNS THE RADIUS OF CURVATURE
// -------------------------------
float Bspline::getRadius(const float& t) const
{
	float r = 1.0f / this->getCurvature(t);

	return r;
}

// CLOCKWISE CHECK
// ---------------
bool Bspline::isClockwise(const float& t) const
{
	CRAB::Vector4Df vup = cross(this->getNormal(t), this->getTangent(t)).to_unitary();
	float dotP = dot(vup, this->getNormalUp(t));
	if (dotP > 0.0f)
		return true;
	else return false;
}