/* [VERT] */ 
#version 330

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

uniform mat4 mvp;


out vec2 UV;
out vec3 normal;

void main()
{
    gl_Position = mvp*vec4(in_Position,1.0);
    UV = in_TexCoord;
    normal = mat3(mvp)*in_Normal;
}
