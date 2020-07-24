#pragma once

#ifndef GRID_H
#define GRID_H

#include <glad\glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "Shader.h"
#include "NURBS.h"

#define STEPS 100

class Grid
{
public:

    struct Vertex {
        // position
        glm::vec3 Position;
        // color
        glm::vec3 Color;
    };

    /*  Mesh Data  */
    std::vector<Vertex> vertices;
    unsigned int VAO;

    /*  Functions  */
    // default constructor
    Grid();
    // constructor
    Grid(std::vector<Vertex> vertices);
    // overload constructor (from NURBS curve)
    Grid(const NURBS& curve);
    // destructor
    ~Grid();

    // render the mesh
    void Draw(Shader shader);

private:
    /*  Render data  */
    unsigned int VBO/*, EBO*/;

    // OpenGL primitive type
    GLenum primitive_type;

    // With points
    bool with_points;

    /*  Functions    */

    // initializes all the buffer objects/arrays
    void setupGrid();
};

#endif