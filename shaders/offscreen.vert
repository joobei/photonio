/* [VERT] */ 
#version 330 /* Specify which version of GLSL we are using. */
in vec3 in_Position; 

uniform mat4 mvp; 
    
void main() 
{ 
   gl_Position = vec4(in_Position, 1.0); 
   gl_Position = mvp * gl_Position; 
} 