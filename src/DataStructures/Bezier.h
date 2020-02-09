#pragma once
#include <vector>
#include <iostream>

#include "Linear_Algebra.h"

namespace CURVE
{
	// Cubic Bézier curves
	struct Bezier
	{
		int segments;
		std::vector<CRAB::Vector4Df> control_points;
		bool enable;

		// DEFAULT CONSTRUCTOR
		Bezier()
		{
			enable = true;
			segments = 50;
			// VERTICAL CURVE
			control_points.push_back({ 0.0f, 10.0f, 0.0f, 1.0f });
			control_points.push_back({ 0.0f, 15.0f, -10.0f, 1.0f });
			control_points.push_back({ 0.0f, 15.0f, -30.0f, 1.0f });
			control_points.push_back({ 0.0f, 10.0f, -40.0f, 1.0f });
			// HORIZONTAL CURVE
			/*control_points.push_back({ 0.0f, 10.0f, 0.0f, 1.0f });
			control_points.push_back({ 0.0f, 10.0f, -20.0f, 1.0f });
			control_points.push_back({ 10.0f, 10.0f, -40.0f, 1.0f });
			control_points.push_back({ 30.0f, 10.0f, -60.0f, 1.0f });*/
		}

		// OVERLOAD CONSTRUCTOR (from point list)
		/*Bezier(const std::vector<CRAB::Vector4Df> &control_points_list)
		{
			enable = false;
			segments = 50;
			for (int i = 0; i < control_points_list.size(); i++)
				control_points.push_back(control_points_list[i]);
		}*/

		CRAB::Vector4Df tan(float t) const
		{
			CRAB::Vector4Df tan_vector;
			tan_vector = control_points[0] * (-3.0 + 6.0 * t - 3.0 * powf(t, 2.0f)) +
				control_points[1] * (3.0 - 12.0 * t + 9.0 * powf(t, 2.0f)) +
				control_points[2] * (6.0 * t - 9.0 * powf(t, 2.0f)) +
				control_points[3] * (3.0 * powf(t, 2.0f));
			return tan_vector.to_unitary();
		}

		CRAB::Vector4Df pos(float t) const
		{
			CRAB::Vector4Df position;
			position = control_points[0] * powf(1.0 - t, 3.0f) +
				control_points[1] * (3 * t * powf(1 - t, 2.0f)) +
				control_points[2] * (3 * (1 - t) * powf(t, 2.0f)) +
				control_points[3] * powf(t, 3.0f);
			return position;
		}

		/*Bezier offset(const CRAB::Vector4Df& o)
		{
			std::vector<CRAB::Vector4Df> control_points_list;

			return Bezier(control_points_list);
		}*/
	};
}
