#pragma once

#include <vector>

#include "Grid.h"
#include "Mesh.h"
#include "BoxGirder.h"

// List of Grids
extern std::vector<Grid> curves;
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
// List of Vehicles
extern std::vector<Vehicle*> vehicles;