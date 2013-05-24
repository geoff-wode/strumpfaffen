layout(location = 0) in vec3 position;

void main()
{
  gl_Position = ModelViewProjection * vec4(position, 1);
}
