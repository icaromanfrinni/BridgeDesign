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

	this->SetupArcLengthTable();
}

// OVERLOAD CONSTRUCTOR (from Segments)
// ------------------------------------
NURBS::NURBS(const std::vector<HorSegment*>& segments)
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
			this->points.push_back(segments[i]->getStartPoint3D());
			this->w.push_back(1.0f);
		}
		this->points.push_back(segments[i]->getMidPoint3D());
		this->w.push_back(segments[i]->midPointWeight());
		this->points.push_back(segments[i]->getEndPoint3D());
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
	std::cout << "number of control points = " << this->points.size() << std::endl;*/

	/*for (int i = 0; i < this->points.size(); i++)
		std::cout << "P[" << i << "] = {" << this->points[i].x << "; " << this->points[i].y << "; " << this->points[i].z << "}" << std::endl;*/

	/*for (int i = 0; i < this->w.size(); i++)
		std::cout << "w[" << i << "] = " << this->w[i] << std::endl;*/

	/*for (int i = 0; i < this->T.size(); i++)
		std::cout << "u[" << i << "] = " << this->T[i] << std::endl;*/

	this->SetupArcLengthTable();
}

// DESTRUCTOR
// ----------
NURBS::~NURBS()
{
}

// SETUP THE ARC LENGTH TABLE
// --------------------------
void NURBS::SetupArcLengthTable()
{
	// comprimento aproximado da curva para construir tabela a cada metro
	float steps = int(this->getChordLength());
	// inicialização da tabela
	arcLength_table.push_back(ArcLength(0.0f, 0.0f));
	
	for (int i = 1; i <= steps; i++)
	{
		// parâmetro 't' atual
		float ti = i / steps;
		float G = 0.0f;
		// Gauss–Legendre quadrature (n = 2 points)
		for (int k = 1; k <= 2; k++)
		{
			float w, alfa;
			if (k == 1)
			{
				w = 1.0f;
				alfa = -1.0f / sqrtf(3.0f);
			}
			else
			{
				w = 1.0f;
				alfa = 1.0f / sqrtf(3.0f);
			}
			float tk = 0.5f * ((ti - arcLength_table.back().t) * alfa + (ti + arcLength_table.back().t));
			CRAB::Vector4Df d1 = this->deriv_4D(tk);
			G += w * d1.length();
		}
		// cálculo do comprimento 's' atual
		float si = arcLength_table.back().s + 0.5f * (ti - arcLength_table.back().t) * G;
		// armazena na tabela
		//std::cout << "t = " << ti << "; s = " << si << std::endl;
		arcLength_table.push_back(ArcLength(ti, si));
	}
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
	glm::vec3 d = (dA * W - A * dW) / powf(W, 2.0f);

	return d;
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
//float NURBS::dX(const float& t) const
//{
//	int span = this->FindSpan(t);
//
//	float A = 0.0f;
//	float dA = 0.0f;
//	float W = 0.0f;
//	float dW = 0.0f;
//	for (int i = 0; i <= this->P; i++)
//	{
//		int index = span - this->P + i;
//		float Base = this->N(index, this->P, t);
//		float dBase = this->dN(index, this->P, t);
//		A += (this->points[index].x * this->w[index]) * Base;
//		dA += (this->points[index].x * this->w[index]) * dBase;
//		W += this->w[index] * Base;
//		dW += this->w[index] * dBase;
//	}
//
//	return (dA * W - A * dW) / powf(W, 2.0f);
//}
//float NURBS::dY(const float& t) const
//{
//	int span = this->FindSpan(t);
//
//	float A = 0.0f;
//	float dA = 0.0f;
//	float W = 0.0f;
//	float dW = 0.0f;
//	for (int i = 0; i <= this->P; i++)
//	{
//		int index = span - this->P + i;
//		float Base = this->N(index, this->P, t);
//		float dBase = this->dN(index, this->P, t);
//		A += (this->points[index].y * this->w[index]) * Base;
//		dA += (this->points[index].y * this->w[index]) * dBase;
//		W += this->w[index] * Base;
//		dW += this->w[index] * dBase;
//	}
//
//	return (dA * W - A * dW) / powf(W, 2.0f);
//}
//float NURBS::dZ(const float& t) const
//{
//	int span = this->FindSpan(t);
//
//	float A = 0.0f;
//	float dA = 0.0f;
//	float W = 0.0f;
//	float dW = 0.0f;
//	for (int i = 0; i <= this->P; i++)
//	{
//		int index = span - this->P + i;
//		float Base = this->N(index, this->P, t);
//		float dBase = this->dN(index, this->P, t);
//		A += (this->points[index].z * this->w[index]) * Base;
//		dA += (this->points[index].z * this->w[index]) * dBase;
//		W += this->w[index] * Base;
//		dW += this->w[index] * dBase;
//	}
//
//	return (dA * W - A * dW) / powf(W, 2.0f);
//}

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

// RETURNS THE DERIVATIVE
// ----------------------
CRAB::Vector4Df NURBS::deriv_4D(const float& t) const
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
	glm::vec3 d = (dA * W - A * dW) / powf(W, 2.0f);

	return CRAB::Vector4Df{ d.x, d.y, d.z, 0.0f };
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

// RETURNS THE CHORD LENGTH OF CURVE
// ---------------------------------
float NURBS::getChordLength() const
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

// RETURNS THE ARC LENGTH OF CURVE
// -------------------------------
float NURBS::getArcLength() const
{
	/*float steps = 100.0f;
	std::vector<float> t;
	std::vector<float> s;
	t.push_back(0.0f);
	s.push_back(0.0f);
	for (int i = 1; i <= steps; i++)
	{
		float ti = i / steps;
		float G = 0.0f;
		for (int k = 1; k <= 2; k++)
		{
			float w, alfa;
			if (k == 1)
			{
				w = 1.0f;
				alfa = -1.0f / sqrtf(3.0f);
			}
			else
			{
				w = 1.0f;
				alfa = 1.0f / sqrtf(3.0f);
			}
			float tk = 0.5f * ((ti - t.back()) * alfa + (ti + t.back()));
			glm::vec3 deriv3D = this->deriv(tk);
			CRAB::Vector4Df deriv4D = { deriv3D.x, deriv3D.y, deriv3D.z, 0.0f };
			G += w * deriv4D.length();
		}
		float si = s.back() + 0.5f * (ti - t.back()) * G;
		t.push_back(ti);
		s.push_back(si);
	}
	return s.back();*/
	return this->arcLength_table.back().s;
}

// RETURNS THE DISTANCE FROM START
// -------------------------------
float NURBS::getDistance(const float& t) const
{
	//std::cout << "NURBS::getDistance(" << t << ")" << std::endl;

	/*float D = 0.0f;
	float steps = 100.0f;
	glm::vec3 A = this->getPosition(0.0f);
	for (int i = 1; i <= steps; i++)
	{
		float u = t * float(i) / steps;
		glm::vec3 B = this->getPosition(u);
		D += glm::distance(A, B);
		A = B;
	}
	return D;*/

	if (t == 0.0f)
		return this->arcLength_table.front().s;
	if (t == 1.0f)
		return this->arcLength_table.back().s;

	int low = 0;
	int high = this->arcLength_table.size();
	int mid = int(high / 2.0f);

	while (t < this->arcLength_table[mid].t || t >= this->arcLength_table[mid].t)
	{
		if (t < this->arcLength_table[mid].t)
			high = mid;
		else low = mid;
		mid = int((low + high) / 2.0f);
		if (low == mid)
			return CRAB::Linear_Interpolation(this->arcLength_table[low].t, this->arcLength_table[low].s, this->arcLength_table[high].t, this->arcLength_table[high].s, t);
		/*if (fabsf(low - mid) < SMALL_NUMBER)
			return this->arcLength_table[mid].s;*/
	}
	return this->arcLength_table[mid].s;
}

// RETURNS THE PARAMETER FROM ARC LENGTH
// -------------------------------------
float NURBS::getParameter(const float& length) const
{
	if (length == 0.0f)
		return this->arcLength_table.front().t;
	if (length >= this->arcLength_table.back().s)
		return this->arcLength_table.back().t;

	int low = 0;
	int high = this->arcLength_table.size();
	int mid = int(high / 2.0f);

	while (length < this->arcLength_table[mid].s || length >= this->arcLength_table[mid].s)
	{
		if (length < this->arcLength_table[mid].s)
			high = mid;
		else low = mid;
		mid = int((low + high) / 2.0f);
		if (low == mid)
			return CRAB::Linear_Interpolation(this->arcLength_table[low].s, this->arcLength_table[low].t, this->arcLength_table[high].s, this->arcLength_table[high].t, length);
		/*if (fabsf(low - mid) < SMALL_NUMBER)
			return this->arcLength_table[mid].s;*/
	}
	return this->arcLength_table[mid].t;
}

// RETURNS THE HORIZONTAL DISTANCE FROM START
// ------------------------------------------
float NURBS::getHorDistance(const float& t) const
{
	float D = 0.0f;

	float steps = 100.0f;
	glm::vec3 A = this->getPosition(0.0f);
	A.y = 0.0f;
	for (int i = 1; i <= steps; i++)
	{
		float u = t * float(i) / steps;
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