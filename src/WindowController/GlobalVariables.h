#pragma once

#include <string>
#include <vector>

#include "Camera.h"

extern Camera camera;

// Initialize input data
// ---------------------
extern std::string bridgeName, roadName;
//extern std::vector<std::string> bridgeTypes;
extern float cross_station, h_clearance, v_clearance, main_span, start_s, end_s;
extern std::vector<float> column_stations;
extern std::string piers_inline;
extern float roadWidth, dSpeed, slopeMax;
extern int nLanes;
extern float nPiers;

// Box Girder Bridge
// -----------------
//extern float web_inclination;
//extern bool iw_Locked;
//extern float cantilever_length, deck_thickness, web_thickness, haunch_height, bottom_width, haunch_width;

// Default Vertical curves values
// ------------------------------
extern float decelRate;			// deceleration rate (m/s²)
extern float reactionTime;		// brake reaction time (seconds)
extern float eyeHeight;			// height of eye above road surface (m)
extern float objHeight;			// height of object above road surface (m)
extern float lightHeight;		// headlight height above road surface (m)
extern float lightAngle;		// angle of light beam intersects the surface of the roadway (degree)