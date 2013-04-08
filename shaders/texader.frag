/* [FRAG] */ 
#version 330

in vec2 UV;
in vec4 shadowProj;

out vec4 fragColor;

uniform sampler2D texturex;
uniform sampler2DShadow shadowMap;

void main()
{
    fragColor = vec4(texture(texturex, UV).rgb,1);

    float shadow = 1.0;
    shadow = textureProj(shadowMap,shadowProj);

    if (shadow != 1.0) {
        fragColor.rgb -=0.4;
    }
    else {
    fragColor *= shadow;
    }
}
