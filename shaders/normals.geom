/* [GEOMETRY] */
#version 330 

uniform mat4 modelMatrix;
uniform mat4 mvp;

in vec3 normal[];
 
out vec3 out_Normal;

layout(triangles) in;

layout(line_strip, max_vertices=6) out;

void main() 
{
	if (length(normal[0]) == 0) {
	
	float normalLength = 2.0f;

    for(int i = 0; i < gl_in.length(); ++i)
  {
		vec4 normal = vec4(normal[i],0.0);

        gl_Position = mvp*gl_in[i].gl_Position;
        EmitVertex();
        gl_Position = mvp*(gl_in[i].gl_Position+normal);
        EmitVertex();
		EndPrimitive();
    }
	   

	   }
} 