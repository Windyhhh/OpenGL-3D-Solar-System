#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <GLFW/glfw3.h>

// Globals for transform state
void InitTransform();
void ComputeGeometricTransform(float q1[3], float q2[3]);
void ApplyGeometricTransform();

#endif
