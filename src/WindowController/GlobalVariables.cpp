#include "GlobalVariables.h"

Camera camera;

// Initialize input data
// ---------------------
std::string bridgeName, roadName;
//std::vector<std::string> bridgeTypes = { "Box Girder", "Beam" };
std::vector<float> column_stations;
std::string piers_inline;
float main_span = 200.0f; // m
float slopeMax = 6.0f;
float cross_station, v_clearance, h_clearance, start_s, end_s;
float roadWidth, dSpeed;
int nLanes = 2;
float nPiers;
float pierDepth = 0.5f; // m

// Box Girder Bridge
// -----------------
//float web_inclination = 0.1f;
//bool iw_Locked = false;
//float cantilever_length, deck_thickness, web_thickness, haunch_height, bottom_width, haunch_width;

// Default Vertical curves values
// ------------------------------
float decelRate = 3.4;		// m/s²
float reactionTime = 2.5;	// seconds
float eyeHeight = 1.08;		// m
float objHeight = 0.60;		// m
float lightHeight = 0.60;	// m
float lightAngle = 1;		// degree