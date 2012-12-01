/* [VERT] */ 
#version 330

in vec3 in_Position;
in vec3 in_Color;

uniform mat4 mvp;

out vec4 color;

void main()
{
    gl_Position = mvp*vec4(in_Position,1.0);
    color = vec4(in_Color,1.0);
}
