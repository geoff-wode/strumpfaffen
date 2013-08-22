
out vec2 vertexTexCoord;

void main()
{
  gl_Position = vec4(Position, 1.0f);

  // Pass the vertex's texture coordinate to the fragment shader...
  vertexTexCoord = TexCoord0;
}
