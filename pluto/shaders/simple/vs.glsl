
out vec3 fragmentNormal;
out vec2 fragmentUV;

void main()
{
  gl_Position = ModelViewProjection * vec4(Position, 1);

  fragmentNormal = Normal;
  fragmentUV = TexCoord0;
}
