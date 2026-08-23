#ifndef MESH_LOADER_H
#define MESH_LOADER_H

typedef struct {
    int nV, nE, nF;
    float *vertices;   // nV x 3
    int   *edges;      // nE x 2
    int   *faces;      // nF x 4 (Max 4 vertices per face)
    int   *faceSizes;  // nF (3 or 4)
} Mesh;

void ReadMesh(const char *fileName, Mesh *meshOut);

#endif
