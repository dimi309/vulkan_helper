#version 420
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 position;

void main()
{
  gl_Position = position;

  // OpenGL -> Vulkan viewport correction
  // see: http://matthewwellings.com/blog/the-new-vulkan-coordinate-system/
  gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;
  gl_Position.y = -gl_Position.y;

}
