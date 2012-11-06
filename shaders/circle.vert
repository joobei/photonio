/* [VERT] */ 
#version 330 /* Specify which version of GLSL we are using. */

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

in vec3 in_Position;
flat out vec4 pos;    

void main() 
{ 
   pos = projection*view*model*vec4(in_Position,1.0);
   gl_Position = projection*view*model*vec4(in_Position,1.0);
 
} 