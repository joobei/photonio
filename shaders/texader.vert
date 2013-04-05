/* [VERT] */ 
#version 330

in vec3 in_Position;
in vec2 in_TexCoord;

uniform mat4 shadowMatrix;
uniform mat4 mvp;

out vec2 UV;
out vec4 shadowProj;

void main()
{
    gl_Position = mvp*vec4(in_Position,1.0);
    shadowProj = shadowMatrix*vec4(in_Position,1.0);
    UV = in_TexCoord;
}
