#pragma once

//#include <string>
//#include <vector>

#include "Camera.h"
#include "Mesh.h"

#include "BoxGirder.h"

// Initialize input data
// ---------------------
extern std::string bridgeName, roadName;
extern std::vector<std::string> bridgeTypes;
extern float cross_station, h_clearance, v_clearance, main_span;
extern float roadWidth, dSpeed;

extern Camera camera;

// List of Meshes
extern std::vector<Mesh> ourMesh_List;
// List of Solids (to load .obj file)
extern std::vector<HED::solid*> solids;
// List of Bridge models
extern std::vector<Bridge*> bridges;
// List of Roadways
extern std::vector<Road*> roadways;
// List of Alignments
extern std::vector<Alignment*> alignments;