/* [VERT] */ 
#version 330

in vec3 in_Position;
in vec3 in_Normal;

out vec3 Vertex_Normal;
out vec4 Vertex_LightDir;
out vec4 Vertex_EyeVec;

uniform mat4 mvp;
uniform mat4 modelview;

uniform vec4 light_position;

void main()
{
    gl_Position = mvp * vec4(in_Position,1);
    Vertex_Normal = mat3(modelview)  * in_Normal;
    vec4 view_vertex = modelview * vec4(in_Position,1);
    Vertex_LightDir = light_position - view_vertex;
    Vertex_EyeVec = -view_vertex;
}
