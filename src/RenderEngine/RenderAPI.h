
#pragma once

#include <iostream>
#include <Windows.h>
#include <chrono>

// sleep_for Function
//#include <thread>
//#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#include "DearImGui.h"
#include "Controller.h"
#include "Shader.h"
#include "DirectionalLight.h"
#include "Grid.h"
#include "Skybox.h"
#include "GlobalTextures.h"

#define EXAMPLE 2
#define DEBUG 1

namespace CRAB
{
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void processInput(GLFWwindow* window);

    // settings
    const unsigned int SCR_WIDTH = 1280;
    const unsigned int SCR_HEIGHT = 720;

    // camera
    //Camera camera;
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;
    float walkAround = 0.0f;

    // projection matrix
    glm::mat4 projection = glm::mat4(1.0f);

    // lighting
    //DirectionalLight mainLight({ 0.9f, 0.9f, 0.9f }, camera.LookAt);
    DirectionalLight mainLight({ 0.9f, 0.9f, 0.9f }, { -1.0f, -1.0f, -1.0f });
    
    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // mouse event handlers
    int TheKeyState = GLFW_KEY_LEFT_CONTROL;

    static void glfw_error_callback(int error, const char* description)
    {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }

    int main()
    {
        // glfw: initialize and configure
        // ------------------------------
        // Setup window
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            return 1;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

        // glfw window creation
        // --------------------
        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CRAB::BRIDGE", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        // configure global opengl state
        // -----------------------------
        glEnable(GL_DEPTH_TEST);

        // Setup Dear ImGui
        // ----------------
        DearImGui::Setup(window);

        // build and compile our shader zprogram
        // -------------------------------------
        Shader ourShader("shaders/shader.vs", "shaders/shader.fs");
        Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.fs");
        Shader gridShader("shaders/grid.vs", "shaders/grid.fs");

        // load grid
        // ---------
        Grid grid_Mesh;

        // load SKYBOX textures
        // --------------------
        std::vector<std::string> faces1 = {
            "skybox/right.jpg",        // right
            "skybox/left.jpg",        // left
            "skybox/top.jpg",        // top
            "skybox/bottom.jpg",        // bottom
            "skybox/front.jpg",        // front
            "skybox/back.jpg"         // back
        };
        std::vector<std::string> faces2 = {
           "skybox/color/side.jpg",        // right
           "skybox/color/side.jpg",        // left
           "skybox/color/top.jpg",        // top
           "skybox/color/bottom.jpg",        // bottom
           "skybox/color/side.jpg",        // front
           "skybox/color/side.jpg"         // back
        };
        std::vector<std::string> faces3 = {
           "skybox/clouds/right.jpg",        // right
           "skybox/clouds/left.jpg",        // left
           "skybox/clouds/top.jpg",        // top
           "skybox/clouds/bottom.jpg",        // bottom
           "skybox/clouds/front.jpg",        // front
           "skybox/clouds/back.jpg"         // back
        };
        Skybox skybox(faces2);

        // load textures
        // -------------
        texPavement.push_back(new Texture("textures/half_pavement_diffuse.jpg", "diffuse"));
        texPavement.push_back(new Texture("textures/half_pavement_roughness.jpg", "specular"));
        texPavement.push_back(new Texture("textures/half_pavement_normal.jpg", "normal"));
        
        texConcrete.push_back(new Texture("textures/Concrete_009_COLOR.jpg", "diffuse"));
        texConcrete.push_back(new Texture("textures/Concrete_009_DISP.jpg", "specular"));
        texConcrete.push_back(new Texture("textures/Concrete_009_NORM.jpg", "normal"));

        // shader configuration
        // --------------------
        skyboxShader.use();
        skyboxShader.setInt("skybox", 0);

        // load models (from OBJ file)
        // ---------------------------
        //std::vector<Mesh> ourMesh_List;
        //ourMesh_List = CRAB::LoadOBJ("objects/cubes.obj");

        // time before
        // -----------
        auto t1 = std::chrono::high_resolution_clock::now();

        // load models
        // -----------
        std::cout << std::endl;
        std::cout << "\tLOAD MODELS" << std::endl;
        std::cout << "\t-----------" << std::endl;
        
        /* EXEMPLO: Reunião 13 */
#if EXAMPLE == 1

        // SEM TRANSIÇÃO
        std::vector<HorSegment*> road_plan;
        road_plan.push_back(new HorSegment(glm::vec3(40.0f, 0.0f, 80.0f), glm::vec3(40.0f, 0.0f, 20.0f)));
        road_plan.push_back(new HorSegment(glm::vec3(40.0f, 0.0f, 20.0f), glm::vec3(40.0f, 0.0f, 0.0f), glm::vec3(20.0f, 0.0f, 0.0f)));
        road_plan.push_back(new HorSegment(glm::vec3(20.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 20.0f)));
        road_plan.push_back(new HorSegment(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 40.0f), glm::vec3(20.0f, 0.0f, 40.0f)));
        road_plan.push_back(new HorSegment(glm::vec3(20.0f, 0.0f, 40.0f), glm::vec3(100.0f, 0.0f, 40.0f)));
        road_plan.push_back(new HorSegment(glm::vec3(100.0f, 0.0f, 40.0f), glm::vec3(120.0f, 0.0f, 40.0f), glm::vec3(120.0f, 0.0f, 60.0f)));
        road_plan.push_back(new HorSegment(glm::vec3(120.0f, 0.0f, 60.0f), glm::vec3(120.0f, 0.0f, 80.0f), glm::vec3(100.0f, 0.0f, 80.0f)));
        road_plan.push_back(new HorSegment(glm::vec3(100.0f, 0.0f, 80.0f), glm::vec3(80.0f, 0.0f, 80.0f), glm::vec3(80.0f, 0.0f, 60.0f)));
        road_plan.push_back(new HorSegment(glm::vec3(80.0f, 0.0f, 60.0f), glm::vec3(80.0f, 0.0f, 0.0f)));

        std::vector<VerSegment*> profile;
        profile.push_back(new VerSegment(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(50.0f, 0.0f, 0.0f)));
        profile.push_back(new VerSegment(glm::vec3(50.0f, 0.0f, 0.0f), glm::vec3(60.0f, 0.0f, 0.0f), glm::vec3(69.94f, 1.06f, 0.0f)));
        profile.push_back(new VerSegment(glm::vec3(69.94f, 1.06f, 0.0f), glm::vec3(144.3f, 8.94f, 0.0f)));
        profile.push_back(new VerSegment(glm::vec3(144.3f, 8.94f, 0.0f), glm::vec3(154.25f, 10.0f, 0.0f), glm::vec3(164.25f, 10.0f, 0.0f)));
        profile.push_back(new VerSegment(glm::vec3(164.25f, 10.0f, 0.0f), glm::vec3(224.25f, 10.0f, 0.0f)));
        profile.push_back(new VerSegment(glm::vec3(224.25f, 10.0f, 0.0f), glm::vec3(234.25f, 10.0f, 0.0f), glm::vec3(244.19f, 8.94f, 0.0f)));
        profile.push_back(new VerSegment(glm::vec3(244.19f, 8.94f, 0.0f), glm::vec3(318.55f, 1.06f, 0.0f)));
        profile.push_back(new VerSegment(glm::vec3(318.55f, 1.06f, 0.0f), glm::vec3(328.5f, 0.0f, 0.0f), glm::vec3(338.5f, 0.0f, 0.0f)));
        profile.push_back(new VerSegment(glm::vec3(338.5f, 0.0f, 0.0f), glm::vec3(388.5f, 0.0f, 0.0f)));

        // alignment
        alignments.push_back(new Alignment("Pista_01", road_plan, profile));
        // road
        roadways.push_back(new Road("Rodovia_001", 8.00f, 40.0f, alignments.back()));
        // bridge
        bridges.push_back(new BoxGirder("Rio_Pacoti", roadways.back(), 180.0f/*, 6.0f*/, 90.0f));
#endif
       
        /* EXEMPLO: Superelevação */
#if EXAMPLE == 2

        std::vector<HorSegment*> road_plan;
        road_plan.push_back(new HorSegment(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 0.0f, 0.0f)));
        road_plan.push_back(new HorSegment(glm::vec3(100.0f, 0.0f, 0.0f), glm::vec3(200.0f, 0.0f, 0.0f), glm::vec3(200.0f, 0.0f, 100.0f)/*, 60.0f, 1.0f / 68.0f, BOTH*/));
        road_plan.push_back(new HorSegment(glm::vec3(200.0f, 0.0f, 100.0f), glm::vec3(200.0f, 0.0f, 200.0f)));

        std::vector<VerSegment*> profile;
        profile.push_back(new VerSegment(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(50.0f, 0.0f, 0.0f)));

        // alignment
        alignments.push_back(new Alignment("Pista_01", road_plan, profile));
        // road
        roadways.push_back(new Road("Rodovia_001", 8.00f, 40.0f, alignments.back()));
        // bridge
        bridges.push_back(new BoxGirder("Rio_Pacoti", roadways.back(), 100.0f/*, 6.0f*/, 50.0f));
#endif

        /* EXEMPLO: Reunião 13 (com cálculo automático do alinhamento vertical) */
#if EXAMPLE == 3
        std::vector<HorSegment*> road_plan;
        road_plan.push_back(new HorSegment(glm::vec3(40.0f, 0.0f, 280.0f), glm::vec3(40.0f, 0.0f, 20.0f)));
        road_plan.push_back(new HorSegment(glm::vec3(40.0f, 0.0f, 20.0f), glm::vec3(40.0f, 0.0f, 0.0f), glm::vec3(20.0f, 0.0f, 0.0f)));
        road_plan.push_back(new HorSegment(glm::vec3(20.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 20.0f)));
        road_plan.push_back(new HorSegment(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 40.0f), glm::vec3(20.0f, 0.0f, 40.0f)));
        road_plan.push_back(new HorSegment(glm::vec3(20.0f, 0.0f, 40.0f), glm::vec3(100.0f, 0.0f, 40.0f)));
        road_plan.push_back(new HorSegment(glm::vec3(100.0f, 0.0f, 40.0f), glm::vec3(120.0f, 0.0f, 40.0f), glm::vec3(120.0f, 0.0f, 60.0f)));
        road_plan.push_back(new HorSegment(glm::vec3(120.0f, 0.0f, 60.0f), glm::vec3(120.0f, 0.0f, 80.0f), glm::vec3(100.0f, 0.0f, 80.0f)));
        road_plan.push_back(new HorSegment(glm::vec3(100.0f, 0.0f, 80.0f), glm::vec3(80.0f, 0.0f, 80.0f), glm::vec3(80.0f, 0.0f, 60.0f)));
        road_plan.push_back(new HorSegment(glm::vec3(80.0f, 0.0f, 60.0f), glm::vec3(80.0f, 0.0f, -200.0f)));

        std::vector<VerSegment*> road_profile;
        road_profile.push_back(new VerSegment(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(788.5f, 0.0f, 0.0f)));

        // alignment
        alignments.push_back(new Alignment("Pista_01", road_plan, road_profile));
        // road
        roadways.push_back(new Road("Rodovia_001", 8.00f, 60.0f, alignments.back()));
        // bridge
        bridges.push_back(new BoxGirder("Rio_Pacoti", roadways.back(), 394.25f, 10.0f, 100.0f));
#endif

        /* EXEMPLO: Viadutos do Cocó */
#if EXAMPLE == 4

        std::vector<HorSegment*> road_plan;
        std::vector<VerSegment*> road_profile;

        /* ----------------- Viaduto 1 ----------------- */
        // road_plan
        road_plan.push_back(new HorSegment(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(238.28f, 0.0f, 439.57f)));
        // road_profile
        road_profile.push_back(new VerSegment(glm::vec3(0.0f, 4.67f, 0.0f), glm::vec3(323.23f, 4.67f, 0.0f)));
        road_profile.push_back(new VerSegment(glm::vec3(323.23f, 4.67f, 0.0f), glm::vec3(345.95f, 4.67f, 0.0f), glm::vec3(366.85f, 5.712f, 0.0f)));
        road_profile.push_back(new VerSegment(glm::vec3(366.85f, 5.712f, 0.0f), glm::vec3(500.0f, 12.348f, 0.0f)));
        // alignment
        alignments.push_back(new Alignment("Pista_1", road_plan, road_profile));
        // road
        roadways.push_back(new Road("Av_Eng_Santana_Jr", 7.00f, 60.0f, alignments.back()));
        // bridge
        bridges.push_back(new BoxGirder("Viaduto_1", roadways.back(), 330.0f, 5.5f, 56.0f));

        /* ----------------- Viaduto 2 ----------------- */
        road_plan.clear();
        road_profile.clear();
        // road_plan
        road_plan.push_back(new HorSegment(glm::vec3(382.06f, 0.0f, 172.76f), glm::vec3(211.03f, 0.0f, 248.89f)));
        road_plan.push_back(new HorSegment(glm::vec3(211.03f, 0.0f, 248.89f), glm::vec3(141.85f, 0.0f, 279.68f), glm::vec3(177.97f, 0.0f, 346.23f)));
        road_plan.push_back(new HorSegment(glm::vec3(177.97f, 0.0f, 346.23f), glm::vec3(287.92f, 0.0f, 548.79f)));
        // road_profile
        road_profile.push_back(new VerSegment(glm::vec3(0.0f, 13.322f, 0.0f), glm::vec3(192.33f, 6.526f, 0.0f)));
        road_profile.push_back(new VerSegment(glm::vec3(192.33f, 6.526f, 0.0f), glm::vec3(249.03f, 4.522f, 0.0f), glm::vec3(303.88f, 6.991f, 0.0f)));
        road_profile.push_back(new VerSegment(glm::vec3(303.88f, 6.991f, 0.0f), glm::vec3(533.14f, 17.311f, 0.0f)));
        // alignment
        alignments.push_back(new Alignment("Pista_2", road_plan, road_profile));
        // road
        roadways.push_back(new Road("Av_Ant_Sales", 8.50f, 40.0f, alignments.back()));
        // bridge
        bridges.push_back(new BoxGirder("Viaduto_2", roadways.back(), 267.0f, 11.0f, 80.0f));

#endif

        /* CASO 1: Viaduct */
#if EXAMPLE == 5
        std::vector<HorSegment*> road_plan;
        std::vector<VerSegment*> road_profile;
        // road_plan
        road_plan.push_back(new HorSegment(glm::vec3(-250.0f, 0.0f, 0.0f), glm::vec3(250.0f, 0.0f, 0.0f)));
        // road_profile
        road_profile.push_back(new VerSegment(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(500.0f, 0.0f, 0.0f)));
        // alignment
        alignments.push_back(new Alignment("Pista", road_plan, road_profile));
        // road
        roadways.push_back(new Road("Rodovia", 8.00f, 40.0f, alignments.back()));
        // bridge
        bridges.push_back(new BoxGirder("Viaduto_1", roadways.back(), 250.0f/*, 5.5f*/, 60.0f));
#endif

        /* CASO 2: Overpass */
#if EXAMPLE == 6
        std::vector<HorSegment*> road_plan;
        std::vector<VerSegment*> road_profile;
        // road_plan
        road_plan.push_back(new HorSegment(glm::vec3(-250.0f, 0.0f, 0.0f), glm::vec3(250.0f, 0.0f, 0.0f)));
        // road_profile
        road_profile.push_back(new VerSegment(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(500.0f, 10.0f, 0.0f)));
        // alignment
        alignments.push_back(new Alignment("Pista_1", road_plan, road_profile));
        // road
        roadways.push_back(new Road("Rodovia_1", 8.00f, 40.0f, alignments.back()));
        // bridge
        bridges.push_back(new BoxGirder("Viaduto_1", roadways.back(), 250.0f, 10.0f, 60.0f, 0.0f));
#endif

        /* CASO 3: Bridge */
#if EXAMPLE == 7
        std::vector<HorSegment*> road_plan;
        std::vector<VerSegment*> road_profile;
        // road_plan
        road_plan.push_back(new HorSegment(glm::vec3(-250.0f, 0.0f, 0.0f), glm::vec3(250.0f, 0.0f, 0.0f)));
        // road_profile
        road_profile.push_back(new VerSegment(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(500.0f, 10.0f, 0.0f)));
        // alignment
        alignments.push_back(new Alignment("Pista_1", road_plan, road_profile));
        // road
        roadways.push_back(new Road("Rodovia_1", 8.00f, 40.0f, alignments.back()));
        // bridge
        bridges.push_back(new BoxGirder("Viaduto_1", roadways.back(), 250.0f, 1.0f, 60.0f, 0.0f, 9.5f));
#endif

        /* Bridge with Terrain */
#if EXAMPLE == 8
        std::vector<HorSegment*> road_plan;
        std::vector<VerSegment*> road_profile;
        // road_plan
        road_plan.push_back(new HorSegment(glm::vec3(-688.40f, 0.0f, 418.69f), glm::vec3(639.13f, 0.0f, -345.17f)));
        // road_profile
        road_profile.push_back(new VerSegment(glm::vec3(0.0f, 37.18f, 0.0f), glm::vec3(1531.61f, 34.54f, 0.0f)));
        // alignment
        alignments.push_back(new Alignment("Pista_1", road_plan, road_profile));
        // road
        roadways.push_back(new Road("Rodovia_1", 15.00f, 60.0f, alignments.back()));
        // bridge
        bridges.push_back(new BoxGirder("Viaduto_1", roadways.back(), 765.81f, 1.0f, 1400.0f, 0.0f, 10.0f));
#endif

        /* Viaduto 13 de Maio with Terrain */
#if EXAMPLE == 9
        std::vector<HorSegment*> road_plan;
        std::vector<VerSegment*> road_profile;
        /* VIADUTO 1 */
        // road_plan
        road_plan.push_back(new HorSegment(glm::vec3(-188.27f, 0.0f, -96.48f), glm::vec3(-96.85f, 0.0f, -21.47f)));
        road_plan.push_back(new HorSegment(glm::vec3(-96.85f, 0.0f, -21.47f), glm::vec3(-35.57f, 0.0f, 28.80f), glm::vec3(43.02f, 0.0f, 18.56f)));
        road_plan.push_back(new HorSegment(glm::vec3(43.02f, 0.0f, 18.56f), glm::vec3(190.27f, 0.0f, -0.64f)));
        // road_profile
        road_profile.push_back(new VerSegment(glm::vec3(0.0f, 3.43f, 0.0f), glm::vec3(110.0f, 9.31f, 0.0f)));
        road_profile.push_back(new VerSegment(glm::vec3(110.0f, 9.31f, 0.0f), glm::vec3(160.0f, 0.0f, 0.0f)));
        road_profile.push_back(new VerSegment(glm::vec3(160.0f, 0.0f, 0.0f), glm::vec3(260.0f, 0.0f, 0.0f)));
        road_profile.push_back(new VerSegment(glm::vec3(260.0f, 0.0f, 0.0f), glm::vec3(400.0f, 9.23f, 0.0f)));
        // alignment
        alignments.push_back(new Alignment("Pista_1", road_plan, road_profile));
        // road
        roadways.push_back(new Road("Av_Treze_de_Maio", 7.00f, 50.0f, alignments.back()));
        // bridge
        bridges.push_back(new BoxGirder("Viaduto_1", roadways.back(), 250.0f, 6.0f, 90.0f));
        /* VIADUTO 2 */
        road_plan.clear();
        road_profile.clear();
        // road_plan
        road_plan.push_back(new HorSegment(glm::vec3(-193.35f, 0.0f, -90.29f), glm::vec3(-101.92f, 0.0f, -15.29f)));
        road_plan.push_back(new HorSegment(glm::vec3(-101.92f, 0.0f, -15.29f), glm::vec3(-37.97f, 0.0f, 37.18f), glm::vec3(44.06f, 0.0f, 26.49f)));
        road_plan.push_back(new HorSegment(glm::vec3(44.06f, 0.0f, 26.49f), glm::vec3(191.31f, 0.0f, 7.29f)));
        // road_profile
        road_profile.push_back(new VerSegment(glm::vec3(0.0f, 3.43f, 0.0f), glm::vec3(110.0f, 9.31f, 0.0f)));
        road_profile.push_back(new VerSegment(glm::vec3(110.0f, 9.31f, 0.0f), glm::vec3(160.0f, 0.0f, 0.0f)));
        road_profile.push_back(new VerSegment(glm::vec3(160.0f, 0.0f, 0.0f), glm::vec3(260.0f, 0.0f, 0.0f)));
        road_profile.push_back(new VerSegment(glm::vec3(260.0f, 0.0f, 0.0f), glm::vec3(400.0f, 9.23f, 0.0f)));
        // alignment
        alignments.push_back(new Alignment("Pista_2", road_plan, road_profile));
        // road
        roadways.push_back(new Road("Av_Treze_de_Maio", 7.00f, 50.0f, alignments.back()));
        // bridge
        bridges.push_back(new BoxGirder("Viaduto_2", roadways.back(), 250.0f, 6.0f, 90.0f));
#endif

        /* Viadutos: AirportFlyover with Terrain */
#if EXAMPLE == 10
        std::vector<HorSegment*> road_plan;
        std::vector<VerSegment*> road_profile;
        /* VIADUTO 1 */
        // road_plan
        road_plan.push_back(new HorSegment(glm::vec3(-40.51f, 0.0f, 348.17f), glm::vec3(2.71f, 0.0f, -112.22f)));
        road_plan.push_back(new HorSegment(glm::vec3(2.71f, 0.0f, -112.22f), glm::vec3(22.92f, 0.0f, -327.59f), glm::vec3(-124.72f, 0.0f, -485.68f)));
        // road_profile
        road_profile.push_back(new VerSegment(glm::vec3(0.0f, 6.93f, 0.0f), glm::vec3(362.0f, 9.49f, 0.0f)));
        road_profile.push_back(new VerSegment(glm::vec3(362.0f, 9.49f, 0.0f), glm::vec3(462.0f, 10.2f, 0.0f), glm::vec3(561.99f, 9.05f, 0.0f)));
        road_profile.push_back(new VerSegment(glm::vec3(561.99f, 9.05f, 0.0f), glm::vec3(869.0f, 5.50f, 0.0f)));
        // alignment
        alignments.push_back(new Alignment("Pista_1", road_plan, road_profile));
        // road
        roadways.push_back(new Road("Southern_Cross_Way", 10.00f, 40.0f, alignments.back()));
        // bridge
        bridges.push_back(new BoxGirder("Viaduto_1", roadways.back(), 400.0, 5.0f, 50.0f));
#endif

        /* Creugenat Viaducts */
#if EXAMPLE == 11
        std::vector<HorSegment*> road_plan;
        std::vector<VerSegment*> road_profile;

        /* VIADUTO 1 */
        // road_plan
        road_plan.push_back(new HorSegment(glm::vec3(-414.27f, 0.0f, -252.65f), glm::vec3(-78.99f, 0.0f, 120.87f), glm::vec3(422.73f, 0.0f, 134.7f)));
        // road_profile
        road_profile.push_back(new VerSegment(glm::vec3(0.0f, 497.49f, 0.0f), glm::vec3(948.1f, 515.21f, 0.0f)));
        // alignment
        alignments.push_back(new Alignment("Pista_1", road_plan, road_profile));
        // road
        roadways.push_back(new Road("Road_1", 10.50f, 60.0f, alignments.back()));
        // bridge
        bridges.push_back(new BoxGirder("Viaduto_1", roadways.back(), 541.21f, 6.0f, 730.0f, 448.25f));

        /* VIADUTO 2 */
        road_plan.clear();
        road_profile.clear();
        // road_plan
        road_plan.push_back(new HorSegment(glm::vec3(-404.3f, 0.0f, -217.8f), glm::vec3(-96.0f, 0.0f, 119.06f), glm::vec3(357.12f, 0.0f, 175.69f)));
        // road_profile
        road_profile.push_back(new VerSegment(glm::vec3(0.0f, 496.68f, 0.0f), glm::vec3(875.12f, 512.38f, 0.0f)));
        // alignment
        alignments.push_back(new Alignment("Pista_2", road_plan, road_profile));
        // road
        roadways.push_back(new Road("Road_2", 10.50f, 60.0f, alignments.back()));
        // bridge
        bridges.push_back(new BoxGirder("Viaduto_2", roadways.back(), 496.5f, 6.0f, 730.0f, 448.25f));
#endif

        /* SVR2020: ROUNDABOUT INTERCHANGE */
#if EXAMPLE == 12
        std::vector<HorSegment*> road_plan;
        std::vector<VerSegment*> road_profile;
        // road_plan
        road_plan.push_back(new HorSegment(glm::vec3(506.63f, 0.0f, -174.31f), glm::vec3(363.37f, 0.0f, -170.8f)));
        road_plan.push_back(new HorSegment(glm::vec3(363.37f, 0.0f, -170.8f), glm::vec3(316.18f, 0.0f, -169.69f), glm::vec3(273.46f, 0.0f, -189.7f)));
        road_plan.push_back(new HorSegment(glm::vec3(273.46f, 0.0f, -189.7f), glm::vec3(74.31f, 0.0f, -283.21f), glm::vec3(134.81f, 0.0f, -71.68f)));
        road_plan.push_back(new HorSegment(glm::vec3(134.81f, 0.0f, -71.68f), glm::vec3(142.52f, 0.0f, -44.72f), glm::vec3(142.52f, 0.0f, -16.68f)));
        road_plan.push_back(new HorSegment(glm::vec3(142.52f, 0.0f, -16.68f), glm::vec3(142.52f, 0.0f, 0.0f)));
        // road_profile
        road_profile.push_back(new VerSegment(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(556.44f, 0.0f, 0.0f)));
        // alignment
        alignments.push_back(new Alignment("Pista", road_plan, road_profile));
        // road
        roadways.push_back(new Road("INDIANA", 8.00f, 50.0f, alignments.back()));
        // bridge
        bridges.push_back(new BoxGirder("Interchange", roadways.back(), 370.0/*, 5.0f*/, 75.0f));
#endif

        // mesh
        // ----
        for (int i = 0; i < bridges.size(); i++)
            for (int j = 0; j < bridges[i]->model.size(); j++)
                ourMesh_List.push_back(Mesh(bridges[i]->model[j]));
       /* std::cout << "\t* Roads" << std::endl;
        for (int i = 0; i < roadways.size(); i++)
            for (int j = 0; j < roadways[i]->model.size(); j++)
                ourMesh_List.push_back(Mesh(roadways[i]->model[j]));*/

        // time after
        // ----------
        auto t2 = std::chrono::high_resolution_clock::now();

        // duration
        // --------
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        std::cout << "\tExecution time:............... " << duration << " ms" << std::endl;

        // pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
        // -----------------------------------------------------------------------------------------------------------
        projection = glm::perspective(glm::radians(camera.FieldOfView), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.10f, 5000.0f);
        //projection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.10f, 1000.0f);

        // render loop
        // -----------
        while (!glfwWindowShouldClose(window))
        {
            // Start the Dear ImGui frame
            // --------------------------
            DearImGui::StartFrame();

            // Show menu
            // ---------
            Controller::ShowDemo();
            Controller::MainMenu(window);

            // polygon mode
            // ------------
            switch (Controller::polygon_mode) {
            case 0: // wireframe
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            case 1: // solid
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
            //case 2: // textured
            //    break;
            default:
                break;
            }

            // init
            // ----
            glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
            //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // per-frame time logic
            // --------------------
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // input
            // -----
            processInput(window);

            // render
            // ------
            ourShader.use();
            ourShader.setVec3("viewPos", camera.Position);

            // light properties
            // ----------------
            // directional light
            //mainLight.direction = camera.LookAt;
            ourShader.setDirLight(mainLight);

            // view/projection transformations
            glm::mat4 view = camera.GetViewMatrix();
            ourShader.setMat4("projection", projection);
            ourShader.setMat4("view", view);

            // world transformation
            glm::mat4 model = glm::mat4(1.0f);
            //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            ourShader.setMat4("model", model);

            // render
            for (int i = 0; i < ourMesh_List.size(); i++)
                ourMesh_List[i].Draw(ourShader);

            // draw grid
            gridShader.use();
            gridShader.setMat4("projection", projection);
            gridShader.setMat4("view", view);
            gridShader.setMat4("model", model);
            if (Controller::grid_on)
                grid_Mesh.Draw(gridShader);

            // draw skybox as last
            glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
            skyboxShader.use();
            view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
            skyboxShader.setMat4("view", view);
            skyboxShader.setMat4("projection", projection);
            // skybox cube
            skybox.Draw(ourShader);
            glDepthFunc(GL_LESS); // set depth function back to default

            // Render dear imgui into screen
            // -----------------------------
            DearImGui::Rendering();

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // Cleanup Dear ImGui
        // ------------------
        DearImGui::Cleanup();

        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        glfwTerminate();
        return 0;
    }

    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void processInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(UP, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(DOWN, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            TheKeyState = GLFW_KEY_LEFT_CONTROL;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            TheKeyState = GLFW_KEY_LEFT_SHIFT;

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            for (int i = 0; i < bridges.size(); i++)
                HED::WriteObjFile(bridges[i]->model);
        }
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        {
            solids.clear();
            ourMesh_List.clear();
            bridges.clear();
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        {
            std::string fileName;
            std::cout << "Enter file name (*.obj): " << std::endl;
            std::cin >> fileName;
            fileName = "objects/" + fileName + ".obj";
            ObjFile inputObjFile;
            if (inputObjFile.ReadObjFile(fileName) == false) {
                MessageBox(NULL, (LPCWSTR)L"File could not be opened!", (LPCWSTR)L"Info",
                    MB_OK | MB_ICONEXCLAMATION);
            }
            else {
                //solids.clear();
                //ourMesh_List.clear();
                for (int i = 0; i < inputObjFile.objectList.size(); i++)
                {
                    // from OBJ -> Mesh
                    // ----------------
                    ourMesh_List.push_back(Mesh(inputObjFile.objectList[i]));
                    // from OBJ -> HalfEdge -> Mesh
                    // ----------------------------
                    //solids.push_back(new HED::solid(i, inputObjFile.objectList[i]));
                    //ourMesh_List.push_back(Mesh(solids[i]));
                }
            }
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        {
            walkAround += deltaTime * 0.1f;
            if (walkAround > 1.0f)
                walkAround = 0.0f;

            CRAB::Vector4Df up = bridges.back()->getNormal(walkAround);
            CRAB::Vector4Df head = bridges.back()->alignment->getPosition(walkAround) + up * 1.50f;
            CRAB::Vector4Df view = head + bridges.back()->alignment->getTangent(walkAround);
            
            camera.Position = glm::vec3(head.x, head.y, head.z);
            camera.View = glm::vec3(view.x, view.y, view.z);
            camera.LookAt = camera.View - camera.Position;
            camera.Up = glm::vec3(up.x, up.y, up.z);
        }
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        {
            walkAround -= deltaTime * 0.1f;
            if (walkAround < 0.0f)
                walkAround = 1.0f;

            CRAB::Vector4Df up = bridges.back()->getNormal(walkAround);
            CRAB::Vector4Df head = bridges.back()->alignment->getPosition(walkAround) + up * 1.50f;
            CRAB::Vector4Df view = head + bridges.back()->alignment->getTangent(walkAround);
            camera.Position = glm::vec3(head.x, head.y, head.z);
            camera.View = glm::vec3(view.x, view.y, view.z);
            camera.LookAt = camera.View - camera.Position;
            camera.Up = glm::vec3(up.x, up.y, up.z);
        }
    }

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);

        // update
        projection = glm::perspective(glm::radians(camera.FieldOfView), (float)width / (float)height, 0.1f, 5000.0f);
        //projection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.10f, 1000.0f);
    }

    // glfw: whenever the mouse moves, this callback is called
    // -------------------------------------------------------
    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
        {
            if (TheKeyState == GLFW_KEY_LEFT_SHIFT)
            {
                float radius = glm::length(camera.View - camera.Position);
                camera.ProcessMouseMovement(xoffset, yoffset);
                camera.View = camera.Position + camera.LookAt * radius;
            }
            if (TheKeyState == GLFW_KEY_LEFT_CONTROL)
            {
                float radius = glm::length(camera.View - camera.Position);
                camera.ProcessMouseMovement(xoffset, yoffset);
                camera.Position = camera.View - camera.LookAt * radius;
            }
        }
    }

    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    // ----------------------------------------------------------------------
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        camera.ProcessMouseScroll(yoffset);
    }
}