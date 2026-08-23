#include "../include/transform.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    float x, y, z;
} Vec3;

// State
static int g_transformActive = 0;
static Vec3 g_translate = {0, 0, 0};
static float g_scaleGeom = 1.0f;
static float g_rotAngle = 0.0f;
static Vec3 g_rotAxis = {1, 0, 0};

void InitTransform() {
    g_transformActive = 0;
}

// Math Helpers
static float Length(Vec3 v) { return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z); }
static Vec3 Subtract(Vec3 a, Vec3 b) { Vec3 r = {a.x - b.x, a.y - b.y, a.z - b.z}; return r; }
static Vec3 Add(Vec3 a, Vec3 b) { Vec3 r = {a.x + b.x, a.y + b.y, a.z + b.z}; return r; }
static Vec3 Scale(Vec3 v, float s) { Vec3 r = {v.x * s, v.y * s, v.z * s}; return r; }
static float Dot(Vec3 a, Vec3 b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
static Vec3 Cross(Vec3 a, Vec3 b) {
    Vec3 r;
    r.x = a.y*b.z - a.z*b.y;
    r.y = a.z*b.x - a.x*b.z;
    r.z = a.x*b.y - a.y*b.x;
    return r;
}

void ComputeGeometricTransform(float q1[3], float q2[3]) {
    Vec3 Q1 = {q1[0], q1[1], q1[2]};
    Vec3 Q2 = {q2[0], q2[1], q2[2]};

    // 1. Translate
    // Center of sphere should be at midpoint of Q1 and Q2
    Vec3 mid = Scale(Add(Q1, Q2), 0.5f);
    g_translate = mid;

    // 2. Scale
    // Unit sphere has radius 1 (Diameter 2). Distance(P1, P2) = 2.
    // Target Distance = Length(Q1 - Q2).
    // Scale Factor = Target / 2.0.
    // Wait, SphereMesh generates radius 1. P1=(0,0,1), P2=(0,0,-1). Dist=2.
    // Correct.
    float dist = Length(Subtract(Q1, Q2));
    g_scaleGeom = dist / 2.0f;

    // 3. Rotate
    // Default axis is Z (0,0,1) pointing to North Pole?
    // Wait, P1 is (0,0,1), P2 is (0,0,-1).
    // Vector P1->P2 is (0,0,-2) (Down).
    // Or do we map P1 to Q1 and P2 to Q2?
    // Vector Q1->Q2.
    // We need to rotate vector (0,0,1) to (Q1-Center)?
    // Or just align Z axis to (Q1 - Q2).
    // Actually, P1 is (0,0,1). P2 is (0,0,-1).
    // Vector P = P1 - P2 = (0,0,2). Direction (0,0,1).
    // Vector Q = Q1 - Q2.
    // We want to rotate (0,0,1) to match direction of (Q1 - Q2).
    
    Vec3 targetDir = Subtract(Q1, Q2);
    // Normalize target
    float len = Length(targetDir);
    if (len < 0.0001f) {
        // Degenerate
        g_rotAngle = 0;
        g_rotAxis = (Vec3){1,0,0};
        g_transformActive = 1;
        return;
    }
    Vec3 v = Scale(targetDir, 1.0f / len);
    
    Vec3 zAxis = {0, 0, 1}; // Original P1 direction relative to center?
    // P1 is on +Z relative to origin. P2 is on -Z.
    // Q1 should correspond to P1.
    // So we align +Z to Q1-Q2 direction.
    
    // Angle = acos(dot(zAxis, v))
    float dot = Dot(zAxis, v);
    // Clamp dot
    if (dot > 1.0f) dot = 1.0f;
    if (dot < -1.0f) dot = -1.0f;
    
    g_rotAngle = acosf(dot) * 180.0f / M_PI;
    
    // Axis = Cross(zAxis, v)
    g_rotAxis = Cross(zAxis, v);
    // If parallel (dot close to 1 or -1), axis is 0.
    if (Length(g_rotAxis) < 0.001f) {
        if (dot < 0.0f) {
            // 180 degrees opposite
            g_rotAxis = (Vec3){1, 0, 0}; // Rotate around X
        } else {
            // Same direction
            g_rotAxis = (Vec3){1, 0, 0}; 
            g_rotAngle = 0;
        }
    }

    g_transformActive = 1;

    printf("Transform Computed:\n");
    printf("  Translate: %.2f %.2f %.2f\n", g_translate.x, g_translate.y, g_translate.z);
    printf("  Scale: %.2f\n", g_scaleGeom);
    printf("  Rotate: %.1f deg around %.2f %.2f %.2f\n", g_rotAngle, g_rotAxis.x, g_rotAxis.y, g_rotAxis.z);
}

void ApplyGeometricTransform() {
    // This function is called inside glMatrixMode(GL_MODELVIEW)
    // It should precede global transforms if we want it to be 'intrinsic' to the object?
    // OR override everything?
    // The prompt says "Geometry Transformation".
    // Usually P' = M * P.
    
    if (g_transformActive) {
        glTranslatef(g_translate.x, g_translate.y, g_translate.z);
        glRotatef(g_rotAngle, g_rotAxis.x, g_rotAxis.y, g_rotAxis.z);
        glScalef(g_scaleGeom, g_scaleGeom, g_scaleGeom);
    }
}
