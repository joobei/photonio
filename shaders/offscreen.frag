/* [FRAG] */
#version 330 /* Specify which version of GLSL we are using. */ 
uniform vec4 baseColor; 

//layout(location=0) out vec4 fragColor;
out vec4 fragColor;

void main() 
{ 
 fragColor = baseColor; 
} 