/* [FRAG] */
#version 330 /* Specify which version of GLSL we are using. */ 
uniform vec4 baseColor; 

out vec4 fragColor;

void main() 
{ 
 fragColor = baseColor; 
} 