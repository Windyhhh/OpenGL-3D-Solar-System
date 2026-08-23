#include "../include/sphere_geometry.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void SphereMesh(int m, int n, const char *fileName) {
    FILE *f = fopen(fileName, "w");
    if (!f) {
        printf("Error opening file for writing: %s\n", fileName);
        return;
    }

    int nV = 2 + (n - 1) * m; 
    int nF = 2 * m + (n - 2) * m; 
    int nE = nV + nF - 2;

    fprintf(f, "%d %d %d\n", nV, nE, nF);

    // 1. Write Vertices
    fprintf(f, "0.000000 0.000000 1.000000\n"); // North
    for (int j = 1; j < n; j++) {
        float phi = M_PI * j / n;
        for (int i = 0; i < m; i++) {
            float theta = 2 * M_PI * i / m;
            float x = sin(phi) * cos(theta);
            float y = sin(phi) * sin(theta);
            float z = cos(phi);
            fprintf(f, "%f %f %f\n", x, y, z);
        }
    }
    fprintf(f, "0.000000 0.000000 -1.000000\n"); // South

    // 2. Write Edges
    // Top pole to ring 1
    for (int i = 0; i < m; i++) fprintf(f, "%d %d\n", 0, 1 + i);
    // Vertical between rings
    for (int j = 1; j < n - 1; j++) {
        int r1_start = 1 + (j - 1) * m;
        int r2_start = 1 + j * m;
        for (int i = 0; i < m; i++) fprintf(f, "%d %d\n", r1_start + i, r2_start + i);
    }
    // Ring n-1 to South pole
    int last_ring_start = 1 + (n - 2) * m;
    int south_idx = nV - 1;
    for (int i = 0; i < m; i++) fprintf(f, "%d %d\n", last_ring_start + i, south_idx);
    
    // Horizontal edges
    for (int j = 1; j < n; j++) {
        int start = 1 + (j - 1) * m;
        for (int i = 0; i < m; i++) {
            int next = (i + 1) % m;
            fprintf(f, "%d %d\n", start + i, start + next);
        }
    }

    // 3. Write Faces
    // Top cap (Triangles)
    for (int i = 0; i < m; i++) {
        fprintf(f, "0 %d %d\n", 1 + i, 1 + (i + 1) % m);
    }
    // Middle strips (Quads)
    for (int j = 1; j < n - 1; j++) {
        int cur_row = 1 + (j - 1) * m;
        int next_row = 1 + j * m;
        for (int i = 0; i < m; i++) {
            int i_next = (i + 1) % m;
            fprintf(f, "%d %d %d %d\n", cur_row + i, next_row + i, next_row + i_next, cur_row + i_next);
        }
    }
    // Bottom cap (Triangles)
    for (int i = 0; i < m; i++) {
        fprintf(f, "%d %d %d\n", last_ring_start + (i + 1) % m, last_ring_start + i, south_idx);
    }

    fclose(f);
    printf("Sphere mesh generated: %s (Standard m=%d, n=%d)\n", fileName, m, n);
}
