#include "../include/mesh_loader.h"
#include <stdio.h>
#include <stdlib.h>

void ReadMesh(const char *fileName, Mesh *meshOut) {
    FILE *f = fopen(fileName, "r");
    if (!f) {
        printf("Error opening file for reading: %s\n", fileName);
        return;
    }

    if (meshOut->vertices) { free(meshOut->vertices); meshOut->vertices = NULL; }
    if (meshOut->edges) { free(meshOut->edges); meshOut->edges = NULL; }
    if (meshOut->faces) { free(meshOut->faces); meshOut->faces = NULL; }
    if (meshOut->faceSizes) { free(meshOut->faceSizes); meshOut->faceSizes = NULL; }

    if (fscanf(f, "%d %d %d", &meshOut->nV, &meshOut->nE, &meshOut->nF) != 3) {
        printf("Invalid header\n");
        fclose(f);
        return;
    }

    meshOut->vertices = (float *)malloc(meshOut->nV * 3 * sizeof(float));
    meshOut->edges = (int *)malloc(meshOut->nE * 2 * sizeof(int));
    meshOut->faces = (int *)malloc(meshOut->nF * 4 * sizeof(int)); 
    meshOut->faceSizes = (int *)malloc(meshOut->nF * sizeof(int));

    for (int i = 0; i < meshOut->nV; i++) {
        fscanf(f, "%f %f %f", 
            &meshOut->vertices[3*i], 
            &meshOut->vertices[3*i+1], 
            &meshOut->vertices[3*i+2]);
    }

    for (int i = 0; i < meshOut->nE; i++) {
        fscanf(f, "%d %d", &meshOut->edges[2*i], &meshOut->edges[2*i+1]);
    }

    char buffer[256];
    fgets(buffer, sizeof(buffer), f); 

    for (int i = 0; i < meshOut->nF; i++) {
        char *line = fgets(buffer, sizeof(buffer), f);
        if (!line) break;
        
        int v1, v2, v3, v4;
        int count = sscanf(line, "%d %d %d %d", &v1, &v2, &v3, &v4);
        
        meshOut->faces[4*i + 0] = v1;
        meshOut->faces[4*i + 1] = v2;
        meshOut->faces[4*i + 2] = v3;
        
        if (count >= 4) {
            meshOut->faces[4*i + 3] = v4;
            meshOut->faceSizes[i] = 4;
        } else {
            meshOut->faces[4*i + 3] = -1; 
            meshOut->faceSizes[i] = 3;
        }
    }

    fclose(f);
    printf("ReadMesh Loaded: %d V, %d E, %d F\n", meshOut->nV, meshOut->nE, meshOut->nF);
}
