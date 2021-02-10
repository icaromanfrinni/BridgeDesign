#include "GlobalVariables.h"

Camera camera;

// Initialize input data
// ---------------------
std::string bridgeName, roadName;
//std::vector<std::string> bridgeTypes = { "Box Girder", "Beam" };
float v_clearance = 6.0f;
float main_span = 70.0;
std::vector<float> column_stations;
float cross_station, h_clearance;
float roadWidth, dSpeed;
int nLanes = 2;
float nPiers;

// Box Girder Bridge
// -----------------
float web_inclination = 0.1f;
//float cantilever_length, deck_thickness, web_thickness, haunch_height, bottom_width, haunch_width;