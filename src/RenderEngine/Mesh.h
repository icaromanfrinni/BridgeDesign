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
};

#endif