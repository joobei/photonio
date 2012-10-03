/* [FRAG] */ 
#version 330

in vec2 UV;
layout(location = 0) out vec3 color;

uniform sampler2D texturex;

void main()
{
    color = texture(texturex, UV).rgb;
}
