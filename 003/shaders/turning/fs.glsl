in vec3 theColour;

out vec4 outFragmentColour;

void main()
{
  outFragmentColour = vec4(theColour,1);
}
