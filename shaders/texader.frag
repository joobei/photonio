/* [FRAG] */ 
#version 330

in vec2 UV;
out vec4 fragColor;;

uniform sampler2D texturex;

void main()
{
    fragColor = vec4(texture(texturex, UV).rgb,1);
}
