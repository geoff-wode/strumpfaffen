// This file contains definitions common to all shader programs.
// It is loaded automatically when the first shader is compiled
// and persists thereafter.

// We specify Shader Model 3.3 for everything...
#version 330

// TODO: Implement shared uniform blocks in the shader infrastructure...
layout (std140) uniform GlobalShaderUniforms
{
	vec4 CameraPos;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	mat4 ViewProjectionMatrix;
};

// Common vertex attributes. To add more attributes, use the following idiom:
//    layout(location = USER_VERTEX_ATTR_LOCATION + n) { blah }
// where 'n' is a zero-based index. E.g.
//    layout(location = USER_VERTEX_ATTR_LOCATION + 0) in vec4 Frobnicate;
//    layout(location = USER_VERTEX_ATTR_LOCATION + 1) in float Encromulation;
// defines 2 vertex attributes above and beyond the standard ones.

layout(location = 0) in vec3 VertexPosition;				// Used for single-precision floats
layout(location = 0) in vec3 VertexPositionHigh;		// Used when emulating double-precision floats
layout(location = 1) in vec3 VertexPositionLow;
layout(location = 2) in vec3 VertexNormal;
layout(location = 3) in vec3 VertexColour;
layout(location = 4) in vec2 VertexTexCoord0;

// !!! Ensure this value is greater than the last layout location given above !!!
#define USER_VERTEX_ATTR_LOCATION (5)

// Various useful constants...
#define PI          3.14159
#define TWO_PI      (PI * 2)
#define PI_OVER_2   (PI * 0.5f)
