
uniform mat4 ModelMatrix;

void main()
{
  gl_Position = ViewProjectionMatrix * ModelMatrix * vec4(VertexPosition,1);
}