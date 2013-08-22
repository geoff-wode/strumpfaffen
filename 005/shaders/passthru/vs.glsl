
out vec3 vertexColour;

void main()
{
  gl_Position = vec4(Position, 1.0f);
  vertexColour = Colour;
}
