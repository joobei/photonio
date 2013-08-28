#version 330 core

in vec3 in_Position;
in vec2 in_Texcoord;

uniform mat4 mvMat;
uniform mat4 pMat;
uniform float radius;
uniform sampler2D diffuseTexture;

void main()
{
        gl_Position = mvMat*vec4(in_Position, 1.0);
}
