#version 330

in vec3 in_Position;
in vec3 in_Normal;
in float PathCoord;

uniform mat4 ModelviewProjection;

out vec3 vPosition;
out vec3 vNormal;
out float vPathCoord;

void main()
{
    gl_Position = ModelviewProjection * vec4(in_Position,1.0);
    vPosition = in_Position.xyz;
    //vNormal = in_Normal;
    vNormal = vec3(0,0,1);
    vPathCoord = PathCoord;
}
