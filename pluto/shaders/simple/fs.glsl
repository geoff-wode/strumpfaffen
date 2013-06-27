in vec2 fragmentUV;

uniform sampler2D textureSampler;

out vec4 colour;

void main()
{
  colour = vec4(texture(textureSampler, fragmentUV).rgb, 1);
}
