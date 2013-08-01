#version 330
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ViewProjectionMatrix;
uniform mat4 ModelViewProjectionMatrix;
uniform vec3 CameraPos;

layout(location = 0) in vec3 Position;      // Used for single-precision floats
layout(location = 0) in vec3 PositionHigh;  // Used when emulating double-precision floats
layout(location = 1) in vec3 PositionLow;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 TexCoord0;
