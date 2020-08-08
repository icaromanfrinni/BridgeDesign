#pragma once

#include <vector>

#include "Mesh.h"
#include "BoxGirder.h"

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