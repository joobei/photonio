/* [GEOMETRY] */
#version 330 

uniform float radius;

layout(points) in;
layout(line_strip, max_vertices = 200)out;

void main() 
{
	//float PI = 3.14159265358979323846264;  //unused
	vec4 pos = vec4(gl_in[0].gl_Position);

    for(float i = 0; i < 6.38 ; i+=0.1)  //generate vertices at positions on the circumference from 0 to 2*pi 
	//for(float i = 0; i < 3.38 ; i+=0.1)  //generate vertices at positions on the circumference from 0 to 2*pi 
	{
        gl_Position = vec4(pos.x+radius*cos(i)),pos.y+radius*sin(i),pos.z,pos.w);
		EmitVertex();        
    }
	EndPrimitive();
} 