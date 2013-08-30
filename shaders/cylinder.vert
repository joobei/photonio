#version 330 core
#extension GL_EXT_gpu_shader4 : enable

in vec3  in_Position;

uniform vec4 color;
uniform float CylinderExt;
uniform vec3  CylinderDirection;
uniform float CylinderRadius;

out vec4 cylinder_color_in;
out vec3 cylinder_direction_in;
out float cylinder_radius_in;
out float cylinder_ext_in;


void main()
{  
  cylinder_color_in = color;
  cylinder_direction_in = normalize(CylinderDirection);
  cylinder_radius_in = CylinderRadius;
  cylinder_ext_in = CylinderExt;
  gl_Position = vec4(in_Position,1.0);
}
