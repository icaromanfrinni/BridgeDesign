#pragma once
#include <vector>
#include "Linear_Algebra.h"

namespace CRAB
{
	struct Ray
	{
		CRAB::Vector4Df origin, direction;
	};

	struct Triangle
	{
		Vector4Df v[3];

		// CONSTRUCTOR
		Triangle(const Vector4Df& p1, const Vector4Df& p2, const Vector4Df& p3)
		{
			v[0] = p1;
			v[1] = p2;
			v[2] = p3;
		}

		// Normal vector
		Vector4Df normal() const
		{
			return cross(v[1] - v[0], v[2] - v[0]);
		}

		// by Barycentric coordinate system
		bool point_in_triangle(const Vector4Df& p) const
		{
			CRAB::Triangle t1(v[0], v[1], p);
			CRAB::Triangle t2(v[1], v[2], p);
			CRAB::Triangle t3(v[2], v[0], p);

			float s1 = dot(t1.normal(), normal());
			float s2 = dot(t2.normal(), normal());
			float s3 = dot(t3.normal(), normal());

			if (s1 < SMALL_NUMBER) return false;
			else if (s2 < SMALL_NUMBER) return false;
			else if (s3 < SMALL_NUMBER) return false;

			return true;
		}
	};

	struct Polygon
	{
		std::vector<Vector4Df> v;

		// Area
		float area() const
		{
			float S = normal().length();
			return S / 2.0f;
		}

		// Normal vector
		Vector4Df normal() const
		{
			Vector4Df N = { 0.0f, 0.0f, 0.0f, 0.0f };

			for (int i = 0; i < v.size() - 1; i++)
				N += cross(v[i], v[i + 1]);
			N += cross(v[v.size() - 1], v[0]);

			return N;
		}

		// TRIANGULATION by Van Gogh’s Algorithm
		/*
			Skiena, Steven S.; Revilla, Miguel A.
			Programming Challenges - The Programming Contest Training Manual
			Chapters 13 and 14
			Springer, 2003
		*/
		// -------------------------------------

		bool ear_Q(const int& i, const int& j, const int& k)
		{
			CRAB::Triangle t(v[i], v[j], v[k]);

			float s = dot(t.normal(), normal());
			if (s < SMALL_NUMBER) return false;

			for (int m = 0; m < v.size(); m++)
			{
				if ((m != i) && (m != j) && (m != k))
					if (t.point_in_triangle(v[m]))
						return false;
			}

			return true;
		}

		void triangulate(std::vector<CRAB::Triangle>& t)
		{
			// Left/Right neighbor indices
			std::vector<int> L, R;

			// Initialization
			for (int i = 0; i < v.size(); i++)
			{
				L.push_back(((i - 1) + v.size()) % v.size());
				R.push_back(((i + 1) + v.size()) % v.size());
			}

			int i = v.size() - 1;
			while (t.size() < (v.size() - 2))
			{
				i = R[i];

				if (ear_Q(L[i], i, R[i]))
				{
					CRAB::Triangle new_T(v[L[i]], v[i], v[R[i]]);
					t.push_back(new_T);
					L[R[i]] = L[i];
					R[L[i]] = R[i];
				}
			}
		}
	};

	struct Curve
	{
		// BÉZIER CURVE
		// Reference:
		/*
			Mortenson, Michael E.
			GEOMETRIC MODELING (3.ed)
			Chapters 4 and 12
			Industrial Press Inc., 2006
		*/

	private:
		// BASIS FUNCTIONS
		int binomialCoefficient(const int& n, const int& i) const
		{
			if (i == 0 || i == n)
				return 1;
			return binomialCoefficient(n - 1, i - 1) + binomialCoefficient(n - 1, i);
		}
		float BernsteinPolynomial(const int& n, const int& i, const float& t) const
		{
			if (i < 0 || i > n)
				return 0;
			int C = binomialCoefficient(n, i);
			return C * powf(t, i) * powf(1 - t, n - i);
		}

		// CALCULATES THE DERIVATIVE FOR A CURVE OF ORDER 'n'
		// Reference: https://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/Bezier/bezier-der.html
		CRAB::Vector4Df deriv(float t) const
		{
			CRAB::Vector4Df d = { 0.0f, 0.0f, 0.0f, 0.0f };
			int n = points.size() - 1;
			if (n >= 1) // at least 2 control points
				for (int i = 0; i < n; i++) {
					d += (points[i + 1] - points[i]) * n * BernsteinPolynomial(n - 1, i, t);
				}
			return d;
		}
		CRAB::Vector4Df deriv2(float t) const
		{
			CRAB::Vector4Df d = { 0.0f, 0.0f, 0.0f, 0.0f };
			int n = points.size() - 1;
			if (n >= 2) // at least 3 control points
				for (int i = 0; i < (n - 1); i++) {
					d += (points[i + 2] - (points[i + 1] * 2.0f) + points[i]) * n * (n - 1) * BernsteinPolynomial(n - 2, i, t);
				}
			return d;
		}

	public:
		// Control Points
		std::vector<CRAB::Vector4Df> points;

		// ADD CONTROL POINTS
		void AddControlPoint(const CRAB::Vector4Df& p)
		{
			points.push_back(p);
		}
		// RETURNS A POINT ON THE CURVE
		CRAB::Vector4Df getPosition(const float& t) const
		{
			CRAB::Vector4Df position = { 0.0f, 0.0f, 0.0f, 1.0f };
			int n = points.size() - 1;
			for (int i = 0; i < points.size(); i++)	{
				position += points[i] * BernsteinPolynomial(n, i, t);
			}
			return position;
		}
		// RETURNS THE CURVE TANGENT
		CRAB::Vector4Df getTangent(const float& t) const
		{
			return deriv(t).to_unitary();
		}
		// RETURNS THE CURVE NORMAL
		CRAB::Vector4Df getNormal(const float& t) const
		{
			CRAB::Vector4Df k = deriv2(t) - deriv(t) * (dot(deriv2(t), deriv(t)) / deriv(t).lengthsq());
			return k.to_unitary();
		}
		// RETURNS THE CURVE NORMAL UP (Yaw vector)
		CRAB::Vector4Df getNormalUp(const float& t) const
		{
			CRAB::Vector4Df vUp = { 0.0f, 1.0f, 0.0f, 0.0f };
			CRAB::Vector4Df vAux = CRAB::cross(vUp, getTangent(t)).to_unitary();
			CRAB::Vector4Df n = CRAB::cross(getTangent(t), vAux).to_unitary();
			return n;
		}
		// RETURNS THE CURVE BINORMAL
		CRAB::Vector4Df getBinormal(const float& t) const
		{
			return CRAB::cross(getTangent(t), getNormal(t)).to_unitary();
		}
		// RETURNS THE CURVATURE
		float getCurvature(const float& t) const
		{
			float k = cross(deriv(t), deriv2(t)).length() / powf(deriv(t).length(), 3.0f);
			return k;
		}
		// RETURNS THE RADIUS OF CURVATURE
		float getRadius(const float& t) const
		{
			float r = 1.0f / getCurvature(t);
			return r;
		}
		// RETURNS THE CURVE LENGTH
		float getLength() const
		{
			float length = 0.0f;
			// TODO
			return length;
		}
		// CLOCKWISE CHECK
		bool isClockwise(const float& t) const
		{
			CRAB::Vector4Df vup = cross(this->getNormal(t), this->getTangent(t)).to_unitary();
			float dotP = dot(vup, this->getNormalUp(t));
			if (dotP > 0.0f)
				return true;
			else return false;
		}
	};
}