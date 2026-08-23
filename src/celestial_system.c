#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../include/sphere_geometry.h"
#include "../include/mesh_loader.h"
#include "../include/graphics_engine.h"
#include "../include/input.h"
#include "../include/transform.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Define globals expected by input.c
float g_rotX = 0, g_rotY = 0;
float g_scale = 1.0f;
float g_panX = 0, g_panY = 0;
int g_renderMode = 2; 
double g_lastX, g_lastY;
int g_isSelected = 0;
// Note: g_isDraggingRot/Pan are static inside input.c, so we don't need to define them here.

Mesh g_mesh = {0};

// Skybox texture for starfield background
static GLuint g_skyboxTexID = 0;

typedef struct Planet Planet;
struct Planet {
    char name[32];
    float distance;     // Distance from parent
    float radius;       // Size
    float orbitSpeed;   // Degrees per frame
    float selfRotSpeed; // Degrees per frame
    float orbitAngle;
    float selfAngle;
    float color[3];     // Fallback color
    GLuint texID;
    
    // Material properties for realistic lighting
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float shininess;
    
    // Flag for special rendering (e.g., has rings)
    int hasRing;
    float ringInnerRadius;
    float ringOuterRadius;
    
    Planet* children;
    int numChildren;
};

// Helper function to set material properties
void SetPlanetMaterial(Planet* p, 
                       float ar, float ag, float ab,
                       float dr, float dg, float db,
                       float sr, float sg, float sb,
                       float shine) {
    p->ambient[0] = ar; p->ambient[1] = ag; p->ambient[2] = ab; p->ambient[3] = 1.0f;
    p->diffuse[0] = dr; p->diffuse[1] = dg; p->diffuse[2] = db; p->diffuse[3] = 1.0f;
    p->specular[0] = sr; p->specular[1] = sg; p->specular[2] = sb; p->specular[3] = 1.0f;
    p->shininess = shine;
}

// Render orbit line for a planet
void RenderOrbit(float radius) {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 0.5f, 0.0f, 0.5f);  // Semi-transparent orange orbit lines
    glLineWidth(2.0f);  // Thicker orbit lines
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++) {
        float angle = i * 2.0f * (float)M_PI / 100.0f;
        glVertex3f(cosf(angle) * radius, 0.0f, sinf(angle) * radius);
    }
    glEnd();
    glDisable(GL_BLEND);
}

// Render all orbit lines for the solar system
void RenderAllOrbits(Planet* parent) {
    for (int i = 0; i < parent->numChildren; i++) {
        Planet* child = &parent->children[i];
        RenderOrbit(child->distance);
    }
}

// Render Saturn's ring
void RenderSaturnRing(float innerRadius, float outerRadius) {
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    
    // Draw ring with gradient color (cyan-magenta)
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= 64; i++) {
        float angle = i * 2.0f * (float)M_PI / 64.0f;
        float x = cosf(angle);
        float z = sinf(angle);

        // Inner edge - cyan
        glColor4f(0.0f, 1.0f, 1.0f, 0.8f);
        glVertex3f(x * innerRadius, 0.0f, z * innerRadius);

        // Outer edge - magenta and more transparent
        glColor4f(1.0f, 0.0f, 1.0f, 0.4f);
        glVertex3f(x * outerRadius, 0.0f, z * outerRadius);
    }
    glEnd();
    
    glPopAttrib();
}

// Solar System Data
Planet g_sun;

void InitSolarSystem() {
    // Initialize Sun - Emissive, no specular
    sprintf(g_sun.name, "Sun");
    g_sun.distance = 0;
    g_sun.radius = 2.0f;
    g_sun.orbitSpeed = 0;
    g_sun.selfRotSpeed = 0.5f;
    g_sun.color[0]=1.0; g_sun.color[1]=1.0; g_sun.color[2]=0.0;
    g_sun.texID = LoadTextureBMP("sun.bmp");
    g_sun.hasRing = 0;
    // Sun uses emission, so material values are less important
    SetPlanetMaterial(&g_sun, 1.0f, 0.9f, 0.3f, 1.0f, 0.9f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f);
    
    // Planets - All 8 planets of the Solar System
    g_sun.numChildren = 8;
    g_sun.children = (Planet*)malloc(sizeof(Planet) * 8);
    
    // 1. Mercury (水星) - Rocky, low specular
    Planet* mercury = &g_sun.children[0];
    sprintf(mercury->name, "Mercury");
    mercury->distance = 3.5f;
    mercury->radius = 0.4f;
    mercury->orbitSpeed = 2.0f;
    mercury->selfRotSpeed = 1.0f;
    mercury->color[0]=0.5f; mercury->color[1]=0.5f; mercury->color[2]=0.5f;
    mercury->texID = LoadTextureBMP("mercury.bmp");
    mercury->numChildren = 0;
    mercury->children = NULL;
    mercury->hasRing = 0;
    SetPlanetMaterial(mercury, 0.2f, 0.2f, 0.2f, 0.6f, 0.6f, 0.6f, 0.1f, 0.1f, 0.1f, 10.0f);

    // 2. Venus (金星) - Thick atmosphere, diffuse
    Planet* venus = &g_sun.children[1];
    sprintf(venus->name, "Venus");
    venus->distance = 4.5f;
    venus->radius = 0.55f;
    venus->orbitSpeed = 1.5f;
    venus->selfRotSpeed = 0.5f;
    venus->color[0]=0.9f; venus->color[1]=0.7f; venus->color[2]=0.3f;
    venus->texID = LoadTextureBMP("venus.bmp");
    venus->numChildren = 0;
    venus->children = NULL;
    venus->hasRing = 0;
    SetPlanetMaterial(venus, 0.3f, 0.25f, 0.1f, 0.9f, 0.7f, 0.4f, 0.2f, 0.15f, 0.1f, 15.0f);

    // 3. Earth (地球) - Oceans reflect light
    Planet* earth = &g_sun.children[2];
    sprintf(earth->name, "Earth");
    earth->distance = 6.0f;
    earth->radius = 0.6f;
    earth->orbitSpeed = 1.0f;
    earth->selfRotSpeed = 5.0f;
    earth->color[0]=0.0f; earth->color[1]=0.3f; earth->color[2]=1.0f;
    earth->texID = LoadTextureBMP("earth.bmp");
    earth->hasRing = 0;
    SetPlanetMaterial(earth, 0.1f, 0.15f, 0.2f, 0.4f, 0.5f, 0.8f, 0.4f, 0.4f, 0.5f, 30.0f);
    
    // Earth's Moon (月球) - Rocky, slight reflection
    earth->numChildren = 1;
    earth->children = (Planet*)malloc(sizeof(Planet));
    Planet* moon = &earth->children[0];
    sprintf(moon->name, "Moon");
    moon->distance = 1.2f;
    moon->radius = 0.2f;
    moon->orbitSpeed = 3.0f;
    moon->selfRotSpeed = 2.0f;
    moon->color[0]=0.8f; moon->color[1]=0.8f; moon->color[2]=0.8f;
    moon->texID = LoadTextureBMP("moon.bmp");
    moon->numChildren = 0;
    moon->children = NULL;
    moon->hasRing = 0;
    SetPlanetMaterial(moon, 0.2f, 0.2f, 0.2f, 0.7f, 0.7f, 0.7f, 0.1f, 0.1f, 0.1f, 5.0f);

    // 4. Mars (火星) - Dusty, low specular
    Planet* mars = &g_sun.children[3];
    sprintf(mars->name, "Mars");
    mars->distance = 8.0f;
    mars->radius = 0.5f;
    mars->orbitSpeed = 0.8f;
    mars->selfRotSpeed = 4.0f;
    mars->color[0]=1.0f; mars->color[1]=0.3f; mars->color[2]=0.1f;
    mars->texID = LoadTextureBMP("mars.bmp");
    mars->numChildren = 0;
    mars->children = NULL;
    mars->hasRing = 0;
    SetPlanetMaterial(mars, 0.3f, 0.1f, 0.05f, 0.8f, 0.3f, 0.1f, 0.1f, 0.05f, 0.02f, 8.0f);

    // 5. Jupiter (木星) - Gas giant, cloud bands
    Planet* jupiter = &g_sun.children[4];
    sprintf(jupiter->name, "Jupiter");
    jupiter->distance = 11.0f;
    jupiter->radius = 1.2f;
    jupiter->orbitSpeed = 0.4f;
    jupiter->selfRotSpeed = 8.0f;
    jupiter->color[0]=0.8f; jupiter->color[1]=0.6f; jupiter->color[2]=0.4f;
    jupiter->texID = LoadTextureBMP("jupiter.bmp");
    jupiter->numChildren = 0;
    jupiter->children = NULL;
    jupiter->hasRing = 0;
    SetPlanetMaterial(jupiter, 0.25f, 0.2f, 0.15f, 0.8f, 0.6f, 0.4f, 0.2f, 0.15f, 0.1f, 20.0f);

    // 6. Saturn (土星) - Gas giant with prominent rings
    Planet* saturn = &g_sun.children[5];
    sprintf(saturn->name, "Saturn");
    saturn->distance = 14.0f;
    saturn->radius = 1.0f;
    saturn->orbitSpeed = 0.3f;
    saturn->selfRotSpeed = 7.0f;
    saturn->color[0]=0.9f; saturn->color[1]=0.8f; saturn->color[2]=0.5f;
    saturn->texID = LoadTextureBMP("saturn.bmp");
    saturn->numChildren = 0;
    saturn->children = NULL;
    // Saturn has rings!
    saturn->hasRing = 1;
    saturn->ringInnerRadius = 1.3f;   // Just outside the planet
    saturn->ringOuterRadius = 2.2f;   // Extends outward
    SetPlanetMaterial(saturn, 0.3f, 0.25f, 0.15f, 0.9f, 0.8f, 0.5f, 0.2f, 0.18f, 0.1f, 18.0f);

    // 7. Uranus (天王星) - Ice giant, bluish with high specular
    Planet* uranus = &g_sun.children[6];
    sprintf(uranus->name, "Uranus");
    uranus->distance = 17.0f;
    uranus->radius = 0.7f;
    uranus->orbitSpeed = 0.2f;
    uranus->selfRotSpeed = 5.0f;
    uranus->color[0]=0.5f; uranus->color[1]=0.8f; uranus->color[2]=0.9f;
    uranus->texID = LoadTextureBMP("uranus.bmp");
    uranus->numChildren = 0;
    uranus->children = NULL;
    uranus->hasRing = 0;
    SetPlanetMaterial(uranus, 0.15f, 0.25f, 0.3f, 0.5f, 0.8f, 0.9f, 0.4f, 0.5f, 0.6f, 50.0f);

    // 8. Neptune (海王星) - Ice giant, deep blue with high specular
    Planet* neptune = &g_sun.children[7];
    sprintf(neptune->name, "Neptune");
    neptune->distance = 20.0f;
    neptune->radius = 0.65f;
    neptune->orbitSpeed = 0.15f;
    neptune->selfRotSpeed = 4.5f;
    neptune->color[0]=0.2f; neptune->color[1]=0.3f; neptune->color[2]=0.9f;
    neptune->texID = LoadTextureBMP("neptune.bmp");
    neptune->numChildren = 0;
    neptune->children = NULL;
    neptune->hasRing = 0;
    SetPlanetMaterial(neptune, 0.1f, 0.12f, 0.3f, 0.2f, 0.3f, 0.9f, 0.5f, 0.5f, 0.7f, 60.0f);
}

void UpdatePlanet(Planet* p) {
    p->orbitAngle += p->orbitSpeed;
    if(p->orbitAngle > 360) p->orbitAngle -= 360;
    
    p->selfAngle += p->selfRotSpeed;
    if(p->selfAngle > 360) p->selfAngle -= 360;

    for(int i=0; i<p->numChildren; i++) {
        UpdatePlanet(&p->children[i]);
    }
}

void RenderPlanet(Planet* p) {
    glPushMatrix();

    // Orbit Rotation (Around Parent)
    glRotatef(p->orbitAngle, 0.0f, 1.0f, 0.0f);
    glTranslatef(p->distance, 0.0f, 0.0f);
    
    // Check if this is the Sun (emissive rendering)
    int isSun = (strcmp(p->name, "Sun") == 0);
    
    // Set up material properties
    if (isSun) {
        // Sun is self-illuminating - use emission
        glDisable(GL_LIGHTING);
        GLfloat sunEmission[] = {1.0f, 0.95f, 0.4f, 1.0f};
        glMaterialfv(GL_FRONT, GL_EMISSION, sunEmission);
    } else {
        // Regular planets use proper lighting
        glEnable(GL_LIGHTING);
        GLfloat noEmission[] = {0.0f, 0.0f, 0.0f, 1.0f};
        glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);
        glMaterialfv(GL_FRONT, GL_AMBIENT, p->ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, p->diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, p->specular);
        glMaterialf(GL_FRONT, GL_SHININESS, p->shininess);
    }
    
    // Render Saturn's ring BEFORE self-rotation (ring doesn't spin with planet)
    if (p->hasRing) {
        glPushMatrix();
        // Tilt the ring (Saturn's axial tilt is about 27 degrees)
        glRotatef(27.0f, 1.0f, 0.0f, 0.0f);
        RenderSaturnRing(p->ringInnerRadius, p->ringOuterRadius);
        glPopMatrix();
    }
    
    // Save state for Children (Children orbit this planet)
    glPushMatrix(); 
        // Self Rotation & Scale for drawing THIS planet
        glRotatef(p->selfAngle, 0.0f, 1.0f, 0.0f); 
        
        glScalef(p->radius, p->radius, p->radius);

        if (p->texID != 0) {
            glColor3f(1.0f, 1.0f, 1.0f);
            RenderMeshWithTexture(&g_mesh, 2, p->texID);
        } else {
            glColor3fv(p->color);
            RenderMeshWithTexture(&g_mesh, 2, 0); // No texture
        }
    glPopMatrix(); // Restore to Planet Center (for children)
    
    // Reset emission for other objects
    if (isSun) {
        GLfloat noEmission[] = {0.0f, 0.0f, 0.0f, 1.0f};
        glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);
        glEnable(GL_LIGHTING);
    }

    // Render Children
    for(int i=0; i<p->numChildren; i++) {
        RenderPlanet(&p->children[i]);
    }

    glPopMatrix();
}

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

int main(void) {
    // 0. Generate & Load Mesh
    SphereMesh(20, 10, "sphere.msh");
    ReadMesh("sphere.msh", &g_mesh);

    // 1. Setup
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Solar System Simulator", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }

    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    InitRenderer(); // Init Texture System
    InitSolarSystem(); // Init Planets
    
    // Load starfield skybox texture
    g_skyboxTexID = LoadTextureBMP("stars.bmp");
    if (g_skyboxTexID == 0) {
        printf("Note: stars.bmp not found. Skybox disabled.\n");
        printf("You can download a starfield panorama and save as stars.bmp in the bin folder.\n");
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Setup lighting - Sun as point light at origin
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    GLfloat light_pos[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Sun is light source (point light)
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    
    // Improved light properties
    GLfloat light_ambient[] = { 0.15f, 0.15f, 0.15f, 1.0f };  // Dim ambient
    GLfloat light_diffuse[] = { 1.0f, 0.95f, 0.8f, 1.0f };    // Warm sunlight
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };    // White specular
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    // Light attenuation for more realistic falloff
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.001f);

    g_panY = 0;
    g_scale = 0.5f; // Zoom out
    g_rotX = 30.0f; // Tilt view

    while (!glfwWindowShouldClose(window)) {
        // Logic
        UpdatePlanet(&g_sun);

        // Render
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.0f, 0.0f, 0.02f, 1.0f); // Very dark blue (fallback if no skybox)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float ratio = width / (float) height;
        // Increased far clipping plane for skybox (200.0)
        glFrustum(-ratio*0.1, ratio*0.1, -0.1, 0.1, 0.1, 200.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // Camera Transform
        glTranslatef(0.0f, 0.0f, -20.0f); // Move back
        glTranslatef(g_panX, g_panY, 0.0f);
        glRotatef(g_rotX, 1.0f, 0.0f, 0.0f);
        glRotatef(g_rotY, 0.0f, 1.0f, 0.0f);
        glScalef(g_scale, g_scale, g_scale);

        // 1. Render Skybox (starfield background) FIRST
        if (g_skyboxTexID != 0) {
            RenderSkybox(g_skyboxTexID, &g_mesh);
        }
        
        // 2. Render Orbit Lines
        glDisable(GL_LIGHTING);
        RenderAllOrbits(&g_sun);
        
        // 3. Update light position (must be done after modelview is set)
        glEnable(GL_LIGHTING);
        glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
        
        // 4. Render Solar System (planets handle their own lighting/emission)
        RenderPlanet(&g_sun);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
