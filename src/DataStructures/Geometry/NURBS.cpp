#include "NURBS.h"

#include <iostream>

// DEFAULT CONSTRUCTOR
// -------------------
NURBS::NURBS()
{
}

// OVERLOAD CONSTRUCTOR (from Points)
// ----------------------------------
NURBS::NURBS(const std::vector<glm::vec3>& _points)
	: points(_points)
{
	// weights
	for (int i = 0; i < this->points.size(); i++)
		this->w.push_back(1.0f);

	// degree
	this->P = 3;

	// knot vector
	int m = points.size() + this->P;
	int n = points.size() - 1;
	for (int i = 0; i <= m; i++)
	{
		if (i <= this->P)
			this->T.push_back(0.0f);
		else if (i > n)
			this->T.push_back(1.0f);
		else
		{
			float u = float(i - this->P) / float(n - this->P + 1);
			this->T.push_back(u);
		}
	}
}

// OVERLOAD CONSTRUCTOR (from Segments)
// ------------------------------------
NURBS::NURBS(const std::vector<Geometry*>& segments)
{
	/* ------------ NON-UNIFORM ------------ */

	/*this->P = 2;
	float distance = 0.0;
	this->T = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < segments.size(); i++)
	{
		if (i == 0)
		{
			this->points.push_back(segments[i]->getStartPoint());
			this->w.push_back(1.0f);
		}
		this->points.push_back(segments[i]->getMidPoint());
		this->w.push_back(segments[i]->midPointWeight());
		this->points.push_back(segments[i]->getEndPoint());
		this->w.push_back(1.0f);
		distance += segments[i]->getLength();
		this->T.insert(T.end(), 2, distance);
	}
	this->T.insert(T.end(), 1, distance);
	for (int i = 0; i < this->T.size(); i++)
		this->T[i] = this->T[i] / distance;*/

	/* ------------ UNIFORM ------------ */

	// control points and weights
	for (int i = 0; i < segments.size(); i++)
	{
		if (i == 0)
		{
			this->points.push_back(segments[i]->getStartPoint());
			this->w.push_back(1.0f);
		}
		this->points.push_back(segments[i]->getMidPoint());
		this->w.push_back(segments[i]->midPointWeight());
		this->points.push_back(segments[i]->getEndPoint());
		this->w.push_back(1.0f);
	}
	// degree
	this->P = this->points.size() - 1;
	if (this->P > 5)
		this->P = 5;
	// knot vector
	int m = points.size() + this->P;
	int n = points.size() - 1;
	for (int i = 0; i <= m; i++)
	{
		if (i <= this->P)
			this->T.push_back(0.0f);
		else if (i > n)
			this->T.push_back(1.0f);
		else
		{
			float u = float(i - this->P) / float(n - this->P + 1);
			this->T.push_back(u);
		}
	}

	/*std::cout << "number of segments = " << segments.size() << std::endl;
	std::cout << "number of control points = " << this->points.size() << std::endl;

	for (int i = 0; i < this->w.size(); i++)
		std::cout << "w[" << i << "] = " << this->w[i] << std::endl;

	for (int i = 0; i < this->T.size(); i++)
		std::cout << "u[" << i << "] = " << this->T[i] << std::endl;*/
}

// DESTRUCTOR
// ----------
NURBS::~NURBS()
{
}

// BASIS FUNCTIONS
// ---------------
float NURBS::N(const int& i, const int& p, const float& t) const
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
float NURBS::dN(const int& i, const int& p, const float& t) const
{
	float left = this->N(i, p - 1, t) * p / (this->T[i + p] - this->T[i]);
	if (isnan(left)) left = 0.0f;

	float right = this->N(i + 1, p - 1, t) * p / (this->T[i + p + 1] - this->T[i + 1]);
	if (isnan(right)) right = 0.0f;

	return left - right;
}
float NURBS::dN2(const int& i, const int& p, const float& t) const
{
	float left = this->dN(i, p - 1, t) * p / (this->T[i + p] - this->T[i]);
	if (isnan(left) || isinf(left)) left = 0.0f;

	float right = this->dN(i + 1, p - 1, t) * p / (this->T[i + p + 1] - this->T[i + 1]);
	if (isnan(right) || isinf(right)) right = 0.0f;

	return left - right;
}

// DERIVATIVES
// -----------
glm::vec3 NURBS::deriv(const float& t) const
{
	int span = this->FindSpan(t);

	glm::vec3 A = { 0.0f, 0.0f, 0.0f };
	glm::vec3 dA = { 0.0f, 0.0f, 0.0f };
	float W = 0.0f;
	float dW = 0.0f;
	for (int i = 0; i <= this->P; i++)
	{
		int index = span - this->P + i;
		float Base = this->N(index, this->P, t);
		float dBase = this->dN(index, this->P, t);
		A += (this->points[index] * this->w[index]) * Base;
		dA += (this->points[index] * this->w[index]) * dBase;
		W += this->w[index] * Base;
		dW += this->w[index] * dBase;
	}

	return (dA * W - A * dW) / powf(W, 2.0f);
}
glm::vec3 NURBS::deriv2(const float& t) const
{
	int span = this->FindSpan(t);

	glm::vec3 A = { 0.0f, 0.0f, 0.0f };
	glm::vec3 dA = { 0.0f, 0.0f, 0.0f };
	glm::vec3 dA2 = { 0.0f, 0.0f, 0.0f };
	float W = 0.0f;
	float dW = 0.0f;
	float dW2 = 0.0f;
	for (int i = 0; i <= this->P; i++)
	{
		int index = span - this->P + i;
		float Base = this->N(index, this->P, t);
		float dBase = this->dN(index, this->P, t);
		float dBase2 = this->dN2(index, this->P, t);
		A += (this->points[index] * this->w[index]) * Base;
		dA += (this->points[index] * this->w[index]) * dBase;
		dA2 += (this->points[index] * this->w[index]) * dBase2;
		W += this->w[index] * Base;
		dW += this->w[index] * dBase;
		dW2 += this->w[index] * dBase2;
	}

	return (((dA2 * W) - (A * dW2)) * W - ((dA * W) - (A * dW)) * 2.0f) / powf(W, 3.0f);
}

// FIND THE ith KNOT SPAN
// ----------------------
int NURBS::FindSpan(const float& t) const
{
	int n = points.size() - 1;
	if (t == T[n + 1])
		return n;

	int low = this->P;
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
glm::vec3 NURBS::getPosition(const float& t) const
{
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	int span = this->FindSpan(t);

	// denominator
	float d = 0.0f;
	for (int j = 0; j <= this->P; j++)
	{
		int index = span - this->P + j;
		d += this->w[index] * this->N(index, this->P, t);
	}

	for (int i = 0; i <= this->P; i++)
	{
		int index = span - this->P + i;
		position += (this->points[index] * this->w[index]) * this->N(index, this->P, t) / d;
	}

	return position;
}

// RETURNS THE CURVE TANGENT
// -------------------------
glm::vec3 NURBS::getTangent(const float& t) const
{
	return glm::normalize(this->deriv(t));
}

// RETURNS THE CURVE NORMAL
// ------------------------
glm::vec3 NURBS::getNormal(const float& t) const
{
	glm::vec3 d1 = this->deriv(t);
	glm::vec3 d2 = this->deriv2(t);
	glm::vec3 n = d2 - d1 * (glm::dot(d2, d1) / powf(glm::length(d1), 2.0f));
	return glm::normalize(n);
}

// RETURNS THE CURVE NORMAL UP (Yaw vector)
// ----------------------------------------
glm::vec3 NURBS::getNormalUp(const float& t) const
{
	glm::vec3 tan = this->getTangent(t);
	glm::vec3 vUp = { 0.0f, 1.0f, 0.0f };
	glm::vec3 vAux = glm::normalize(glm::cross(vUp, tan));
	glm::vec3 n = glm::normalize(glm::cross(tan, vAux));
	return n;
}

// RETURNS THE CURVE BINORMAL
// --------------------------
glm::vec3 NURBS::getBinormal(const float& t) const
{
	return glm::normalize(glm::cross(this->getTangent(t), this->getNormal(t)));
}

// RETURNS THE CURVATURE
// ---------------------
float NURBS::getCurvature(const float& t) const
{
	glm::vec3 d1 = this->deriv(t);
	glm::vec3 d2 = this->deriv2(t);
	float k = glm::length(glm::cross(d1, d2)) / powf(glm::length(d1), 3.0f);
	return k;
}

// RETURNS THE RADIUS OF CURVATURE
// -------------------------------
float NURBS::getRadius(const float& t) const
{
	float k = this->getCurvature(t);
	if (k < SMALL_NUMBER)
		return INFINITY;
	return 1 / k;
}

// RETURNS THE LENGTH OF CURVE
// ---------------------------
float NURBS::getLength() const
{
	float L = 0.0f;

	glm::vec3 A = this->getPosition(0.0f);
	for (int i = 1; i <= ELEMENTS; i++)
	{
		float t = float(i) / ELEMENTS;
		glm::vec3 B = this->getPosition(t);
		L += glm::distance(A, B);
		A = B;
	}
	return L;
}

// RETURNS THE DISTANCE FROM START
// -------------------------------
float NURBS::getDistance(const float& t) const
{
	float D = 0.0f;

	glm::vec3 A = this->getPosition(0.0f);
	for (int i = 1; i <= /*ELEMENTS*/1000; i++)
	{
		float u = t * float(i) / /*ELEMENTS*/1000;
		glm::vec3 B = this->getPosition(u);
		D += glm::distance(A, B);
		A = B;
	}
	return D;
}

// RETURNS THE HORIZONTAL DISTANCE FROM START
// ------------------------------------------
float NURBS::getHorDistance(const float& t) const
{
	float D = 0.0f;

	glm::vec3 A = this->getPosition(0.0f);
	A.y = 0.0f;
	for (int i = 1; i <= /*ELEMENTS*/1000; i++)
	{
		float u = t * float(i) / /*ELEMENTS*/1000;
		glm::vec3 B = this->getPosition(u);
		B.y = 0.0f;
		D += glm::distance(A, B);
		A = B;
	}
	return D;
}

// CLOCKWISE CHECK
// ---------------
bool NURBS::isClockwise(const float& t) const
{
	glm::vec3 vup = glm::normalize(glm::cross(this->getNormal(t), this->getTangent(t)));
	float dotP = glm::dot(vup, this->getNormalUp(t));
	if (dotP > 0.0f)
		return true;
	else return false;
}