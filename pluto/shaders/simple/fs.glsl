out vec3 fragmentNormal;
in vec2 fragmentUV;

uniform sampler2D textureSampler;

out vec4 colour;

void main()
{
  //colour = vec4(texture(textureSampler, fragmentUV).rgb, 1);
  colour = vec4(0,1,0,1);
}
