#pragma once

#include <vector>

#include "Grid.h"
#include "Mesh.h"
#include "BoxGirder.h"

namespace CRAB
{
	struct Model
	{
		std::string name;
		std::vector<Mesh> mesh_list;

		// CONSTRUCTOR
		Model(const std::string _name)
			: name(_name)
		{
		}
		Model(const std::string _name, const std::vector<HED::solid*> _solids)
			: name(_name)
		{
			for (int i = 0; i < _solids.size(); i++)
				mesh_list.push_back(Mesh(_solids[i]));
		}
	};
}

// List of Grids
extern std::vector<Grid> curves;
// List of Models
extern std::vector<CRAB::Model> models;
// List of Meshes
//extern std::vector<Mesh> ourMesh_List;
// List of Solids (to load .obj file)
//extern std::vector<HED::solid*> solids;
// List of Bridge models
extern std::vector<Bridge*> bridges;
// List of Roadways
extern std::vector<Road*> roadways;
// List of Alignments
extern std::vector<Alignment*> alignments;
// List of Vehicles
extern std::vector<Vehicle*> vehicles;