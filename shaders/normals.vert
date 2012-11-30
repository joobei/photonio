/* [VERT] */
#version 330

in vec3 in_Position;
in vec3 in_Normal;

uniform mat4 mvp;

out VertexData {
	vec3 pos;
	vec3 normal;
} outData;

void main()
{
    gl_Position = mvp*vec4(in_Position,1);

	outData.pos = in_Position;
	outData.normal = in_Normal;
}
