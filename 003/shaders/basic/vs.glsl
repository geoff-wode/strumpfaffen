out vec3 theColour;

void main()
{
  gl_Position = vec4(VertexPosition,1);
  theColour = VertexColour;
}
