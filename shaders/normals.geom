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
layout(line_strip, max_vertices=6) out;

void main() 
{

 //vec3 face_normal = normalize (cross (gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz,gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz));

        for(int i = 0; i < gl_in.length(); ++i)
        {

			/*gl_Position = gl_in[i].gl_Position;
			color = vec3(0,0,1);
			EmitVertex();

			gl_Position = gl_in[i].gl_Position+vec4(face_normal,0);
			color = vec3(1,0,0);
			EmitVertex();
			EndPrimitive();*/

            gl_Position = mvp*vec4(inData[i].pos,1.0);
            color = vec3(0,0,1);
			EmitVertex();

            gl_Position = mvp*vec4(inData[i].pos+inData[i].normal*0.2,1);
            color = vec3(1,0,0);
			EmitVertex();
            EndPrimitive();

        }
} 