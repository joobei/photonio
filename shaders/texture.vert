/* [VERT] */ 
#version 330

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

uniform mat4 mvp;
uniform mat4 modelview;  //to rotate the normals
uniform mat4 model;
uniform mat4 view;


out vec2 UV;
out vec3 normal;

void main()
{
    gl_Position = mvp*vec4(in_Position,1.0);
    UV = in_TexCoord;
    normal = normalize(mat3(model)*normalize(in_Normal));
}
