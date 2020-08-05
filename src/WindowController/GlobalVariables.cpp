#include "GlobalVariables.h"

// Initialize input data
// ---------------------
std::string bridgeName, roadName;
std::vector<std::string> bridgeTypes = { "Box Girder", "Beam" };
float cross_station, h_clearance, v_clearance, main_span;
float roadWidth, dSpeed;

Camera camera;

// List of Meshes
std::vector<Mesh> ourMesh_List;
// List of Solids (to load .obj file)
std::vector<HED::solid*> solids;
// List of Bridge models
std::vector<Bridge*> bridges;
// List of Roadways
std::vector<Road*> roadways;
// List of Alignments
std::vector<Alignment*> alignments;