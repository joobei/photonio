/* [VERT] */ 
#version 330 /* Specify which version of GLSL we are using. */


uniform mat4 pvm;

in vec3 in_Position;

void main() 
{ 

   gl_Position = pvm*vec4(in_Position,1.0);
 
} 