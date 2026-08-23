#include "../include/input.h"
#include "../include/transform.h"
#include "../include/graphics_engine.h"
#include <stdio.h>
#include <math.h>

// Extern globals from main.c
extern float g_rotX, g_rotY;
extern float g_scale;
extern float g_panX, g_panY;
extern int g_renderMode; 
extern double g_lastX, g_lastY;
extern int g_isSelected;

// Control State
static int g_isDraggingRot = 0; // Left Button
static int g_isDraggingPan = 0; // Right/Middle Button
static double g_clickStartX = 0, g_clickStartY = 0;
static int g_hasMoved = 0; 

// Defaults
static const float DEF_ROTX=0, DEF_ROTY=0, DEF_SCALE=1.0f, DEF_PANX=0, DEF_PANY=0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) return;

    if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);
    
    // F1: Reset
    if (key == GLFW_KEY_F1) {
        printf("System Reset (F1)\n");
        g_rotX = DEF_ROTX; g_rotY = DEF_ROTY;
        g_scale = DEF_SCALE;
        g_panX = DEF_PANX; g_panY = DEF_PANY;
        g_renderMode = 2;
        InitTransform(); 
        g_isSelected = 0;
    }

    // Features
    if (key == GLFW_KEY_F2) { ToggleShadows(); printf("Toggle Shadows\n"); }
    if (key == GLFW_KEY_F3) { ToggleTexture(); printf("Toggle Texture\n"); }

    // Render Modes
    if (key == GLFW_KEY_1) { g_renderMode = 0; printf("Mode: Points\n"); }
    if (key == GLFW_KEY_2) { g_renderMode = 1; printf("Mode: Edges\n"); }
    if (key == GLFW_KEY_3) { g_renderMode = 2; printf("Mode: Faces\n"); }
    
    // Geometric Transform
    if (key == GLFW_KEY_T) {
        printf("\n--- Geometric Transform ---\n");
        printf("Enter Q1 (x y z): ");
        float q1[3];
        scanf("%f %f %f", &q1[0], &q1[1], &q1[2]);
        printf("Enter Q2 (x y z): ");
        float q2[3];
        scanf("%f %f %f", &q2[0], &q2[1], &q2[2]);
        ComputeGeometricTransform(q1, q2);
    }

    // --- New Controls (Task 6 Refined) ---
    // Rotation: Arrows
    if (key == GLFW_KEY_UP)    g_rotX -= 5.0f;
    if (key == GLFW_KEY_DOWN)  g_rotX += 5.0f;
    if (key == GLFW_KEY_LEFT)  g_rotY -= 5.0f;
    if (key == GLFW_KEY_RIGHT) g_rotY += 5.0f;

    // Pan: WASD
    if (key == GLFW_KEY_W) g_panY += 0.1f;
    if (key == GLFW_KEY_S) g_panY -= 0.1f;
    if (key == GLFW_KEY_A) g_panX -= 0.1f;
    if (key == GLFW_KEY_D) g_panX += 0.1f;

    // Scale: +/-
    if (key == GLFW_KEY_EQUAL || key == GLFW_KEY_KP_ADD) {
        g_scale *= 1.1f;
    }
    if (key == GLFW_KEY_MINUS || key == GLFW_KEY_KP_SUBTRACT) {
        g_scale *= 0.9f;
        if (g_scale < 0.1f) g_scale = 0.1f;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    g_scale += (float)yoffset * 0.1f;
    if (g_scale < 0.1f) g_scale = 0.1f;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        glfwGetCursorPos(window, &g_lastX, &g_lastY);
        g_clickStartX = g_lastX;
        g_clickStartY = g_lastY;
        g_hasMoved = 0;

        if (button == GLFW_MOUSE_BUTTON_LEFT) g_isDraggingRot = 1;
        if (button == GLFW_MOUSE_BUTTON_RIGHT || button == GLFW_MOUSE_BUTTON_MIDDLE) g_isDraggingPan = 1;
    } 
    else if (action == GLFW_RELEASE) {
        // Check for Click (Pick) on Left Button Release if not moved significantly
        // Note: User prompt implies "Click UI" or "Pick Object". 
        // Standard behavior: Left Click = Pick.
        if (button == GLFW_MOUSE_BUTTON_LEFT && g_isDraggingRot) {
            double mx, my;
            glfwGetCursorPos(window, &mx, &my);
            double dist = sqrt(pow(mx - g_clickStartX, 2) + pow(my - g_clickStartY, 2));
            if (dist < 5.0) { // Small threshold for static click
                 // Perform Pick
                 int w, h;
                 glfwGetWindowSize(window, &w, &h);
                 double cx = w/2.0;
                 double cy = h/2.0;
                 double centerDist = sqrt(pow(mx - cx, 2) + pow(my - cy, 2));
                 if (centerDist < 150 * g_scale) {
                     g_isSelected = !g_isSelected;
                     printf("Object Selected: %s\n", g_isSelected ? "TRUE" : "FALSE");
                 }
            }
        }

        if (button == GLFW_MOUSE_BUTTON_LEFT) g_isDraggingRot = 0;
        if (button == GLFW_MOUSE_BUTTON_RIGHT || button == GLFW_MOUSE_BUTTON_MIDDLE) g_isDraggingPan = 0;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    double dx = xpos - g_lastX;
    double dy = ypos - g_lastY;

    if (g_isDraggingRot) {
        g_rotY += (float)dx * 0.5f;
        g_rotX += (float)dy * 0.5f;
    }
    if (g_isDraggingPan) {
        // Pan logic: roughly pixel to world units
        g_panX += (float)dx * 0.005f; 
        g_panY -= (float)dy * 0.005f; 
    }

    g_lastX = xpos;
    g_lastY = ypos;
}
