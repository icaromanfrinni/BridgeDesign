#pragma once

#ifndef MESH_H
#define MESH_H

#include <glad\glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "ObjFile.h"
#include "HalfEdge.h"
#include "Material.h"
#include "GlobalTextures.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class Mesh
{
public:

    struct Vertex {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
    };

    /*  Mesh Data  */
    std::vector<Vertex> vertices;
    Material material;
    unsigned int VAO;

    /*  Functions  */
    // constructor
    Mesh(std::vector<Vertex> vertices);
    // constructor (from OBJ struct)
    Mesh(const obj& object);
    // constructor (from HalfEdge struct)
    Mesh(const HED::solid* solid);
	// destructor
	~Mesh();

    // render the mesh
    void Draw(Shader shader);

private:
    /*  Render data  */
    unsigned int VBO/*, EBO*/;

    /*  Functions    */
    
    // initializes all the buffer objects/arrays
    void setupMesh();
    
    // Manual calculation of tangents
    glm::vec3 Calc_Tangent(const Vertex& v1, const Vertex& v2, const Vertex& v3);
};

#endif