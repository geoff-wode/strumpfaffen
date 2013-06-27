layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 fragmentUV;

void main()
{
  gl_Position = ModelViewProjection * vec4(position, 1);
  fragmentUV = texCoord;
}
