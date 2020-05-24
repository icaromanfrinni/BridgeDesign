#pragma once
#ifndef OBJFILE_H
#define OBJFILE_H

#include <string>
#include <vector>
#include "Linear_Algebra.h"

//------------------------------------------------------------------------------
// TYPES
//------------------------------------------------------------------------------

struct objLine
{
	int vStart, vEnd;
};

struct objFace
{
	std::vector<int> vertices;
	std::vector<int> textures;
	std::vector<int> normals;
};

struct obj
{
	std::string Name;
	std::vector<CRAB::Vector4Df> Vertices;
	std::vector<CRAB::Vector4Df> vTexture;
	std::vector<CRAB::Vector4Df> vNormals;
	std::vector<objLine> Lines;
	std::vector<objFace> Faces;
};

//------------------------------------------------------------------------------
// CLASSES
//------------------------------------------------------------------------------

class ObjFile
{
public:
	//vector of objets from OBJ File
	std::vector<obj> objectList;

	//DEFAULT CONSTRUCTOR
	ObjFile();
	//DESTRUCTOR
	~ObjFile();

	//CLEAR A OBJ VARIABLE TYPE
	void ClearCurrentObj(obj &_obj);
	//READ .OBJ FILE
	bool ReadObjFile(const std::string& inName);
	//WRITE .OBJ FILE
	//void WriteObjFile(const std::string& name);
};

#endif // OBJFILE_H