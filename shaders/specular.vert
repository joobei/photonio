/* [VERT] */
#version 330

in vec3 in_Position;
in vec3 in_Color;
in vec3 in_Normal;

uniform mat4 modelMatrix;
uniform mat4 mvp;
uniform int numberOfTextures;

out vec4 ex_Color;
out vec3 ex_Normal;

void main()
{
    gl_Position = mvp*vec4(in_Position,1.0);
    ex_Color = vec4(in_Color,1.0);
	ex_Normal = (modelMatrix*vec4(in_Normal,0.0)).xyz;
}
