/* VERTEX */

#version 330

uniform mat4 vm;
uniform mat4 projection;
uniform vec3 view;

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

out vec4 vertexPos;
out vec2 TexCoord;
out vec3 Normal;

void main()
{
	Normal = normalize(vec3(vm * vec4(in_Normal,0.0)));	
	TexCoord = vec2(in_TexCoord);
	
	gl_Position = projection * vm * vec4(in_Position,1.0);
} 