#include "Mesh.h"
#include "Comp_Geometry.h"

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
    if (solid->name == "TOP_LAYER")
    {
        material.hasTexture = true;

        material.textures.push_back(new Texture("textures/pavement_diffuse.jpg", "diffuse"));
        material.textures.push_back(new Texture("textures/pavement_roughness.jpg", "specular"));
        material.textures.push_back(new Texture("textures/pavement_normal.jpg", "normal"));

        /*material.textures.push_back(new Texture("textures/brickwall_diffuse.jpg", "diffuse"));
        material.textures.push_back(new Texture("textures/brickwall_diffuse.jpg", "specular"));
        material.textures.push_back(new Texture("textures/brickwall_normal.jpg", "normal"));*/
    }
    else
    {
        material.hasTexture = true;

        material.textures.push_back(new Texture("textures/concrete_diffuse.png", "diffuse"));
        material.textures.push_back(new Texture("textures/concrete_specular.jpg", "specular"));
        material.textures.push_back(new Texture("textures/concrete_normal.jpg", "normal"));

        /*material.textures.push_back(new Texture("textures/white_concrete_diffuse.jpg", "diffuse"));
        material.textures.push_back(new Texture("textures/white_concrete_specular.jpg", "specular"));
        material.textures.push_back(new Texture("textures/white_concrete_normal.jpg", "normal"));*/
    }
        
    for (int i = 0; i < solid->faces.size(); i++)
    {
        //// check if current face is a triangle
        //if (solid->faces[i]->hEdge->next->next != solid->faces[i]->hEdge->prev) continue;

        //Mesh::Vertex vertex;
        //HED::halfEdge* he = solid->faces[i]->hEdge;

        //// Position
        //vertex.Position = glm::vec3(he->vStart->point.x, he->vStart->point.y, he->vStart->point.z);
        //// Normal
        //CRAB::Vector4Df P1P2 = he->next->vStart->point - he->vStart->point;
        //CRAB::Vector4Df P1P3 = he->next->next->vStart->point - he->vStart->point;
        //CRAB::Vector4Df vertex_normal = CRAB::cross(P1P2, P1P3).to_unitary();
        //vertex.Normal = glm::vec3(vertex_normal.x, vertex_normal.y, vertex_normal.z);

        //vertices.push_back(vertex);

        //for (he = solid->faces[i]->hEdge->next; he != solid->faces[i]->hEdge; he = he->next)
        //{
        //    if (he->next == he->opp) break;
        //    // Position
        //    vertex.Position = glm::vec3(he->vStart->point.x, he->vStart->point.y, he->vStart->point.z);

        //    vertices.push_back(vertex);
        //}

        // Initialize Polygon
        CRAB::Polygon p;
        HED::halfEdge* he = solid->faces[i]->hEdge;
        p.v.push_back(he->vStart->point);
        for (he = solid->faces[i]->hEdge->next; he != solid->faces[i]->hEdge; he = he->next)
        {
            p.v.push_back(he->vStart->point);
        }

        // Triangulate
        std::vector<CRAB::Triangle> t;
        p.triangulate(t);

        // MESH
        for (int j = 0; j < t.size(); j++)
        {
            Mesh::Vertex vertex1, vertex2, vertex3;
            // ------------------------------------
            glm::vec3 v_normal = glm::vec3(t[j].normal().to_unitary().x, t[j].normal().to_unitary().y, t[j].normal().to_unitary().z);
            // ------------------------------------
            vertex1.Normal = v_normal;
            vertex2.Normal = v_normal;
            vertex3.Normal = v_normal;
            // ------------------------------------
            vertex1.Position = glm::vec3(t[j].v[0].x, t[j].v[0].y, t[j].v[0].z);
            vertex2.Position = glm::vec3(t[j].v[1].x, t[j].v[1].y, t[j].v[1].z);            
            vertex3.Position = glm::vec3(t[j].v[2].x, t[j].v[2].y, t[j].v[2].z);
            // ------------------------------------
            vertex1.TexCoords = { 1.0f, 0.0f };
            /*if (j % 2 == 0)
            {
                vertex2.TexCoords = { 5.0f, 0.0f };
                vertex3.TexCoords = { 5.0f, 1.0f };
            }
            else
            {
                vertex2.TexCoords = { 5.0f, 1.0f };
                vertex3.TexCoords = { 0.0f, 1.0f };
            }*/
            /*if (j % 2 == 0)
            {
                vertex2.TexCoords = { 1.0f, 0.0f };
                vertex3.TexCoords = { 1.0f, 1.0f };
            }
            else
            {
                vertex2.TexCoords = { 1.0f, 1.0f };
                vertex3.TexCoords = { 0.0f, 1.0f };
            }*/
            if (j % 2 == 0)
            {
                vertex2.TexCoords = { 1.0f, 0.5f };
                vertex3.TexCoords = { 0.0f, 0.5f };
            }
            else
            {
                vertex2.TexCoords = { 1.0f, 0.5f };
                vertex3.TexCoords = { 0.0f, 0.0f };
            }
            // ------------------------------------
            glm::vec3 tangent = Calc_Tangent(vertex1, vertex2, vertex3);
            vertex1.Tangent = tangent;
            vertex2.Tangent = tangent;
            vertex3.Tangent = tangent;
            // ------------------------------------
            vertices.push_back(vertex1);            
            vertices.push_back(vertex2);
            vertices.push_back(vertex3);
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
    // material properties
    shader.setFloat("material.shininess", material.shininess);
    if (material.hasTexture)
    {
        shader.setBool("material.hasTexture", true);

        for (int i = 0; i < material.textures.size(); i++)
        {
            // active proper texture unit before binding
            glActiveTexture(GL_TEXTURE0 + i);

            if (material.textures[i]->type == "diffuse")
                shader.setInt("u_Texture_diffuse", i);
            else if (material.textures[i]->type == "specular")
                shader.setInt("u_Texture_specular", i);
            else if (material.textures[i]->type == "normal")
                shader.setInt("u_Texture_normal", i);

            // bind the texture
            glBindTexture(GL_TEXTURE_2D, material.textures[i]->Texture_ID);
        }
    }
    else
    {
        shader.setBool("material.hasTexture", false);

        shader.setVec3("material.ka", material.ka);
        shader.setVec3("material.kd", material.kd);
        shader.setVec3("material.ks", material.ks);
    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
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
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

    glBindVertexArray(0);
}

glm::vec3 Mesh::Calc_Tangent(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
    glm::vec3 t;

    glm::vec3 edge1 = v2.Position - v1.Position;
    glm::vec3 edge2 = v3.Position - v1.Position;
    glm::vec2 deltaUV1 = v2.TexCoords - v1.TexCoords;
    glm::vec2 deltaUV2 = v3.TexCoords - v1.TexCoords;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    t.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    t.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    t.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    t = glm::normalize(t);

    return t;
}