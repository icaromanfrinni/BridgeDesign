#include "GlobalVariables.h"

Camera camera;

// Initialize input data
// ---------------------
std::string bridgeName, roadName;
//std::vector<std::string> bridgeTypes = { "Box Girder", "Beam" };
float v_clearance = 5.0f;
float main_span = 35.0f;
float cross_station, h_clearance;
float roadWidth, dSpeed;
int nLanes = 2;
//float cantilever_length, deck_thickness, web_thickness, haunch_height, bottom_width, haunch_width;
float nPiers;

// Design Vehicle
// --------------
float front_overhang = 1.5f;	// front overhang of inner lane vehicle, m
float wheelbase = 2.5f;			// wheelbase of single unit or tractor, m
//float u = 2.0f;				// track width on tangent (out-to-out of tires), m