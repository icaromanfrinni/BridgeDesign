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
extern float roadWidth, dSpeed;
extern int nLanes;
extern float nPiers;

// Box Girder Bridge
// -----------------
extern float web_inclination;
//extern float cantilever_length, deck_thickness, web_thickness, haunch_height, bottom_width, haunch_width;