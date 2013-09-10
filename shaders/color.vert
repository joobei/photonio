#version 330

in vec3 in_Position;
in vec3 in_Color;

uniform mat4 mvp;
uniform float alpha;

out vec3 color;

void main() {
    gl_Position = mvp * vec4(in_Position,1);
    color = in_Color;
    //gl_PointSize = 30-gl_Position.z;
}
