#version 330

// Declare a block of uniform variables that is shared between
// all shaders. I.e., a single chnage to any variable in the block
// is reflected in all shaders.
layout (std140) uniform CommonVarsBlock {
  mat4 ModelMatrix;
  mat4 ViewMatrix;
  mat4 ProjectionMatrix;
  mat4 ViewProjectionMatrix;
  mat4 ModelViewProjectionMatrix;
  vec3 CameraPos;
};

// Common vertex shader inputs:
layout(location = 0) in vec3 VertexPosition;      // Used for single-precision floats
layout(location = 0) in vec3 VertexPositionHigh;  // Used when emulating double-precision floats
layout(location = 1) in vec3 VertexPositionLow;
layout(location = 2) in vec3 VertexNormal;
layout(location = 3) in vec3 VertexTexCoord0;
