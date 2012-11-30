/* [GEOMETRY] */
#version 330 

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 mvp;

out vec3 color;

in VertexData {
	vec3 pos;
	vec3 normal;
} inData[];

layout(triangles) in;
layout(points, max_vertices=6) out;

void main() 
{
        for(int i = 0; i < gl_in.length(); ++i)
        {
            gl_Position = mvp*vec4(inData[i].pos,1.0);
            color = vec3(0,0,1);
			EmitVertex();
			EndPrimitive();


            gl_Position = mvp*vec4(inData[i].pos,1)+(mat3(mvp)*inData[i].normal,1);
            color = vec3(1,0,0);
			EmitVertex();
            EndPrimitive();
        }
} 