#include "GlobalVariables.h"

// Initialize input data
// ---------------------
std::string bridgeName = "Bridge_1";
std::string roadName = "Road_1";
std::vector<std::string> bridgeTypes = { "Box Girder", "Beam" };
float cross_station = 250.0f;
float h_clearance = 60.0f;
float v_clearance = 5.5f;
float main_span = 35.0f;
float roadWidth = 8.0f;
float dSpeed = 40.0f;

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