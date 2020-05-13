#pragma once
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
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
}

class Geometry
{
public:
	// DEFAULT CONSTRUCTOR
	Geometry();
	// DESTRUCTOR
	virtual ~Geometry() = 0;

	// RETURN
	virtual glm::vec3 getStartPoint() const = 0;
	virtual glm::vec3 getMidPoint() const = 0;
	virtual glm::vec3 getEndPoint() const = 0;
	virtual CRAB::Vector4Df getStart4DPoint() const = 0;
	virtual CRAB::Vector4Df getMid4DPoint() const = 0;
	virtual CRAB::Vector4Df getEnd4DPoint() const = 0;
	virtual float getLength() const = 0;
	virtual float getY(const float& x) const = 0;
	virtual float midPointWeight() const = 0;
};

#endif // GEOMETRY_H