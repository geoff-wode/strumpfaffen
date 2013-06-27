#version 330
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 ViewProjection;
uniform mat4 ModelViewProjection;
uniform mat4 World;
uniform mat4 InvWorld;
uniform vec3 CameraPos;

layout(location = 0) in vec3 Position;
layout(location = 0) in vec3 PositionHigh;  // Used when implementing DSFUN90 high precision
layout(location = 1) in vec3 PositionLow;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 TexCoord0;
