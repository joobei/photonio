/* [VERT] */
#version 330

in vec3 in_Position;
in vec3 in_Normal;

out vec4 color;
out vec3 normal;

void main()
{
    gl_Position = vec4(in_Position,1);
	normal = in_Normal;
}
