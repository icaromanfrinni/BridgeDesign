#include "GlobalVariables.h"

Camera camera;

// Initialize input data
// ---------------------
std::string bridgeName, roadName;
//std::vector<std::string> bridgeTypes = { "Box Girder", "Beam" };
std::vector<float> column_stations;
std::string piers_inline;
float main_span, cross_station, v_clearance, h_clearance, start_s, end_s;
float roadWidth, dSpeed;
int nLanes = 2;
float nPiers;

// Box Girder Bridge
// -----------------
float web_inclination = 0.1f;
//float cantilever_length, deck_thickness, web_thickness, haunch_height, bottom_width, haunch_width;