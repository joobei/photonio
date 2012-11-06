/* [GEOMETRY] */
#version 330 

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
#define pi 3.141592653589793238462643383279

layout(points) in;
layout(line_strip, max_vertices = 40)out;

void main() 
{
    vec4 pos = vec4(0,0,0,1);
    pos = projection*view*model*pos;

    for(float i = 0; i < 6.28 ; i+=0.4)
	 {
        gl_Position = vec4(pos.x+0.5*cos(i),pos.y+0.5*sin(i),pos.z,1.0);
		EmitVertex();        
    }
  
} 