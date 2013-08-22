// Shader uniforms:
uniform vec3 brickColour = { 0.5f, 0.15, 0.15f };
uniform vec3 mortarColour = { 0.5f, 0.5f, 0.5f };
uniform float mortarThickness = 0.01f;
uniform vec2 brickSize = { 0.25f, 0.08f };

//---------------------------------------------------------------
// Vertex shader input attributes:
in vec2 vertexTexCoord;

//---------------------------------------------------------------
// Fragment shader outputs:
out vec4 fragColour;

//---------------------------------------------------------------
// Some code to render a brick-like pattern.
//
// Bricks are defined thus:
//                             #  #
//      =======================#  #======================       ^
//  0: ------------|-------------------------|--------   mortar_thickness
//      =========# | #=====================# | #=============   v
//               # | #                     # | #      ^
//               # | #                     # | #      |
//               # | # <-- brick_width --> # | # brick_height
//               # | #                     # | #      |
//               # | #                     # | #      v
//      =========# | #=====================# | #=============
//  1: ------------|-------------------------|--------
//      ===========|==========#  #===========|===============
//                 |          #  #           |
//                 0                         1

void main()
{
  vec2 combinedSize = brickSize + mortarThickness;

  // get a position within a single brick...
  vec2 inBrick = vertexTexCoord / combinedSize;

  // offset horizontally by half a brick on alternate rows...
  if (mod(inBrick.y * 0.5f, 1.0f) >= 0.5f) { inBrick.x += 0.5f; }

  // determine which brick we're in...
  vec2 whichBrick = floor(inBrick);

  // ...and compute a texture coordinate within a single brick...
  inBrick -= whichBrick;

  // Compute a horizontal and vertical "pulse" that defines whether or not
  // we're inside a brick (pulse == 1) or not (pulse == 0).
  vec2 mortarFrac = mortarThickness * 0.5f / combinedSize;
  vec2 pulse = step(mortarFrac, inBrick) - step(1 - mortarFrac, inBrick);

  // If the horizontal and vertical pulses are both non-zero, then the point
  // is within the body of the brick, otherwise it's in the mortar...
  fragColour = vec4(mix(mortarColour, brickColour, pulse.x * pulse.y), 1);
}
