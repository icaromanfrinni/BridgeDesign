#include "GlobalLists.h"

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