#pragma once

#ifndef MESH_H
#define MESH_H

#include <glad\glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
};

class Mesh
{
public:
    /*  Mesh Data  */
    std::vector<Vertex> vertices;
    unsigned int VAO;

    /*  Functions  */
    // constructor
    Mesh(std::vector<Vertex> vertices);
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