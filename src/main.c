#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "../include/sphere_geometry.h"
#include "../include/mesh_loader.h"
#include "../include/graphics_engine.h"
#include "../include/input.h"
#include "../include/transform.h"

// --- Global State Definitions ---
Mesh g_mesh = {0};
float g_rotX = 0, g_rotY = 0;
float g_scale = 1.0f;
float g_panX = 0, g_panY = 0;
int g_renderMode = 2; // 0: Point, 1: Line, 2: Fill
double g_lastX, g_lastY;
int g_isDragging = 0;
int g_isSelected = 0;

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

int main(void) {
    // 0. Generate Mesh
    SphereMesh(20, 10, "sphere.msh");

    // 1. Read Mesh
    ReadMesh("sphere.msh", &g_mesh);

    // 2. Setup Window
    if (!glfwInit()) return -1;
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Sphere Mesh Viewer (Refactored)", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    // Init Renderer (Textures)
    InitRenderer();

    // 3. Setup OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    
    GLfloat light_pos[] = { 5.0f, 5.0f, 10.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(0.05f, 0.15f, 0.25f, 1.0f);  // Dark blue background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float ratio = width / (float) height;
        glFrustum(-ratio*0.1, ratio*0.1, -0.1, 0.1, 0.1, 100.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glTranslatef(0.0f, 0.0f, -3.0f);
        glTranslatef(g_panX, g_panY, 0.0f);
        glRotatef(g_rotX, 1.0f, 0.0f, 0.0f);
        glRotatef(g_rotY, 0.0f, 1.0f, 0.0f);
        glScalef(g_scale, g_scale, g_scale);

        // Task 4: Apply Geometric Transform (Q1->Q2 Mapping)
        ApplyGeometricTransform();

        RenderScene(&g_mesh, g_renderMode, g_isSelected);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    
    // Cleanup
    if (g_mesh.vertices) free(g_mesh.vertices);
    if (g_mesh.edges) free(g_mesh.edges);
    if (g_mesh.faces) free(g_mesh.faces);
    if (g_mesh.faceSizes) free(g_mesh.faceSizes);

    return 0;
}
