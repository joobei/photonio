/* [VERT] */ 
#version 330

in vec3 in_Position;
in vec3 in_Color;

uniform mat4 pvm;

out vec4 ex_color;

void main()
{
    gl_Position = pvm*vec4(in_Position,1.0);
    ex_color = vec4(in_Color,1.0);
}
