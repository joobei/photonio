/* [FRAG] */
#version 330

in vec4 color;
in vec3 normal;

out vec4 fragColor;

uniform float alpha;
uniform vec4 materialColor;

void main()
{
        fragColor = vec4(1,1,1,1);
}
