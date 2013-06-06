/* [VERT] */
#version 330

in vec3 in_Position;
in vec3 in_Color;
in vec3 in_Normal;

uniform mat4 modelMatrix;
uniform mat4 mvp;

out vec4 color;
out vec3 normal;

void main()
{
    gl_Position = mvp*vec4(in_Position,1.0);
    color = vec4(in_Color,1.0);
        normal = mat3(mvp)*in_Normal;
}
