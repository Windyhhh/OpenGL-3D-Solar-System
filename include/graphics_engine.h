#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H

#include <GLFW/glfw3.h>
#include "mesh_loader.h"

// Initialize textures, etc.
void InitRenderer();

// Toggle flags
void ToggleShadows();
void ToggleTexture();

// Texture Management
GLuint LoadTextureBMP(const char* path);

// Rendering
void RenderPoints(const Mesh* mesh);
void RenderEdges(const Mesh* mesh);
void RenderFaces(const Mesh* mesh);
void RenderMesh(const Mesh* mesh, int mode); 
void RenderMeshWithTexture(const Mesh* mesh, int mode, GLuint texID);

// Advanced Scene Rendering (Task 5)
void RenderScene(const Mesh* mesh, int mode, int isSelected);

// Skybox rendering for starfield background
void RenderSkybox(GLuint skyTexID, const Mesh* sphereMesh);

#endif
