uniform float angle;

out vec3 theColour;

void main()
{
  vec4 offset = vec4( cos(angle), sin(angle), 0.0f, 0.0f) * 0.5f;

  gl_Position = vec4(VertexPosition, 1) + offset;
  theColour = VertexColour;
}
