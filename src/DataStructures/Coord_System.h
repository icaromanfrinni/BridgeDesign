#pragma once
#include "Linear_Algebra.h"

namespace CRAB
{
	struct Station
	{
		int E;
		float meters, elevation;

		// CONSTRUCTOR
		Station() {
			E = 0;
			meters = 0.0f;
			elevation = 0.0f;
		}
		// OVERLOAD CONSTRUCTOR
		Station(const int& _E, const float& _meters) : E(_E), meters(_meters) {
			elevation = 0.0f;
		}
		// OVERLOAD CONSTRUCTOR
		Station(const int& _E, const float& _meters, const float& _elevation) : E(_E), meters(_meters), elevation(_elevation)
		{
		}
		// OVERLOAD CONSTRUCTOR (from Vector4Df)
		Station(const Vector4Df& p) {
			E = int(p.x / 20.0f);
			meters = fmod(p.x, 20.0f);
			elevation = p.y;
		}

		// ALLOCATION (from Vector4Df)
		void setStation(const Vector4Df& p) {
			E = int(p.x / 20.0f);
			meters = fmod(p.x, 20.0f);
			elevation = p.y;
		}
		// ALLOCATION (from another Station)
		void setStation(const CRAB::Station& s) {
			E = s.E;
			meters = s.meters;
			elevation = s.elevation;
		}

		// STATION TO METERS
		inline float c_Meters() const {
			return E * 20.0f + meters;
		}

		// GET DISTANCE BETWEEN TWO STATIONS
		inline float getDistance(const CRAB::Station& s) const {
			return fabsf(s.c_Meters() - c_Meters());
		}

		// GET POINT
		inline Vector4Df getPoint() const {
			return Vector4Df{ c_Meters(), elevation, 0.0f, 1.0f };
		}
	};

	struct UTM
	{
		float N, E;
	};
}