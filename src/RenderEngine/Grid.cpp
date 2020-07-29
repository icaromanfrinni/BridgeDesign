#include "Grid.h"

// default constructor
Grid::Grid()
{
    Grid::Vertex p;
    glm::vec3 color;
    int gridSize = 100;
    // XZ-axis
    for (int i = -gridSize; i <= gridSize; i += 5)
        for (int k = -gridSize; k <= gridSize; k += 5)
        {
            if (i == 0) color = { 0.0f, 0.0f, 0.5f };
            else if (k == 0) color = { 0.5f, 0.0f, 0.0f };
            else color = { 0.5f, 0.5f, 0.5f };

            p.Position = { i, 0.0f, -gridSize };  p.Color = color; this->vertices.push_back(p);
            p.Position = { i, 0.0f, gridSize };  p.Color = color; this->vertices.push_back(p);
            p.Position = { -gridSize, 0.0f, k };  p.Color = color; this->vertices.push_back(p);
            p.Position = { gridSize, 0.0f, k };  p.Color = color; this->vertices.push_back(p);
        }
    // Y-axis
    color = { 0.0f, 0.5f, 0.0f };
    p.Position = { 0.0f, 0.0f, 0.0f }; p.Color = color; this->vertices.push_back(p);
    p.Position = { 0.0f, 1.0f, 0.0f }; p.Color = color; this->vertices.push_back(p);

    this->primitive_type = GL_LINES;
    this->with_points = false;
    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupGrid();
}
// constructor
Grid::Grid(std::vector<Grid::Vertex> vertices)
{
    this->vertices = vertices;
    this->primitive_type = GL_LINES;
    this->with_points = false;
    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupGrid();
}
// overload constructor (from NURBS curve)
Grid::Grid(const NURBS& curve)
{
    this->primitive_type = GL_LINE_STRIP;
    this->with_points = true;

    Grid::Vertex v;
    //v.Color = { 1.0f, 0.0f, 1.0f }; // magenta
    v.Color = { 0.0f, 0.0f, 0.0f }; // preta

    for (int i = 0; i <= STEPS; i++)
    {
        float t = float(i) / STEPS;
        v.Position = curve.getPosition(t);
        this->vertices.push_back(v);
    }

    setupGrid();
}
// destructor
Grid::~Grid()
{
}

// render the Grid
void Grid::Draw(Shader shader)
{
    // draw Grid
    glBindVertexArray(VAO);
    glDrawArrays(this->primitive_type, 0, vertices.size());
    if (this->with_points)
        glDrawArrays(GL_POINTS, 0, vertices.size());
}

// initializes all the buffer objects/arrays
void Grid::setupGrid()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex Color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

    glBindVertexArray(0);
}
