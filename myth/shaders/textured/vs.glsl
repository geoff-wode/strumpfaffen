
in vec3 Position;
in vec2 TextureCoord;

out vec2 fragTextureCoord;

void main()
{
  gl_Position = vec4(Position, 1.0f);

  fragTextureCoord = TextureCoord;
}
