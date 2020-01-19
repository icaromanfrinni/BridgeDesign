#include "Mesh.h"

// constructor
Mesh::Mesh(std::vector<Mesh::Vertex> vertices)
{
    this->vertices = vertices;

    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupMesh();
}
// constructor (from OBJ struct)
Mesh::Mesh(const obj& object)
{
    for (int i = 0; i < object.Faces.size(); i++)
        for (int j = 0; j < object.Faces[i].vertices.size(); j++)
        {
            unsigned int index;
            Mesh::Vertex vertex;

            index = object.Faces[i].vertices[j] - 1;
            vertex.Position = glm::vec3(object.Vertices[index].x, object.Vertices[index].y, object.Vertices[index].z);
            index = object.Faces[i].normals[j] - 1;
            vertex.Normal = glm::vec3(object.vNormals[index].x, object.vNormals[index].y, object.vNormals[index].z);
            
            vertices.push_back(vertex);
        }

    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupMesh();
}
// constructor (from HalfEdge struct)
Mesh::Mesh(const HED::solid* solid)
{
    for (int i = 0; i < solid->faces.size(); i++)
    {
        //DEBUG
        std::cout << "face ... " << solid->faces[i]->id << std::endl;
        std::cout << "face->hEdge ... " << solid->faces[i]->hEdge->id << std::endl;
        std::cout << "\n" << std::endl;

        // check if current face is a triangle
        if (solid->faces[i]->hEdge->next->next != solid->faces[i]->hEdge->prev) continue;

        Mesh::Vertex vertex;
        HED::halfEdge* he = solid->faces[i]->hEdge;

        // Position
        vertex.Position = glm::vec3(he->vStart->point.x, he->vStart->point.y, he->vStart->point.z);
        // Normal
        CRAB::Vector4Df P1P2 = he->next->vStart->point - he->vStart->point;
        CRAB::Vector4Df P1P3 = he->next->next->vStart->point - he->vStart->point;
        CRAB::Vector4Df vertex_normal = CRAB::cross(P1P2, P1P3).to_unitary();
        vertex.Normal = glm::vec3(vertex_normal.x, vertex_normal.y, vertex_normal.z);

        vertices.push_back(vertex);

        for (he = solid->faces[i]->hEdge->next; he != solid->faces[i]->hEdge; he = he->next)
        {
            if (he->next == he->opp) break;
            // Position
            vertex.Position = glm::vec3(he->vStart->point.x, he->vStart->point.y, he->vStart->point.z);

            vertices.push_back(vertex);
        }
    }

    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupMesh();
}
// destructor
Mesh::~Mesh()
{
}

// render the mesh
void Mesh::Draw(Shader shader)
{
    // draw mesh
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
}

// initializes all the buffer objects/arrays
void Mesh::setupMesh()
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
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glBindVertexArray(0);
}