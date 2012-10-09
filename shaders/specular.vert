/* [VERT] */
#version 330

in vec3 in_Position;
in vec3 in_Color;
in vec3 in_Normal;
in vec2 in_bumpMap;

uniform mat4 MV;
uniform mat4 MVP;
uniform int numberOfTextures;

out vec4 ex_color;
out vec3 ex_normal;

void main()
{


    gl_Position = mvp*vec4(in_Position,1.0);
    ex_color = vec4(in_Color,1.0);
}
