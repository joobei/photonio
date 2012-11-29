#version 330

uniform mat4 pvm;

in vec4 in_Position;

void main(void)
{
	gl_Position = pvm*in_Position;
}
