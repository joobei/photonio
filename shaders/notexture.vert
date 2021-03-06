/* [VERT] */ 
#version 330

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

out vec2 Vertex_UV;
out vec3 Vertex_Normal;
out vec4 Vertex_LightDir;
out vec4 Vertex_EyeVec;

out float gl_ClipDistance[1];
uniform vec4 ClipPlane;

uniform mat4 mvp;
uniform mat4 modelview;
uniform mat4 model;

uniform vec4 light_position;
//uniform vec4 uv_tiling;

void main()
{
    gl_Position = mvp * vec4(in_Position,1);
    Vertex_UV = in_TexCoord * vec2(1);  //hack because i don't know what uv_tiling is
    Vertex_Normal = mat3(modelview)  * in_Normal;
    vec4 view_vertex = modelview * vec4(in_Position,1);
    Vertex_LightDir = light_position - view_vertex;
    Vertex_EyeVec = -view_vertex;

    gl_ClipDistance[0] = dot(model * vec4(in_Position,1), ClipPlane);
}
