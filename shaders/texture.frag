/* [FRAG] */ 
#version 330

in vec2 UV;
in vec3 normal;

out vec4 fragColor;

uniform sampler2D texturex;

void main()
{

    vec3 lightDirection = normalize(vec3(0,0,1));
    float ambientIntensity = 0.2;
    vec4 diffuseLightColor = vec4(1.0);

    vec4 diffuseColor;

    vec4 ambientFactor = vec4(texture(texturex, UV).rgb,1)*ambientIntensity;

    float diffuseFactor = dot(normal, -lightDirection);

    if (diffuseFactor > 0) {
                diffuseColor =  vec4(texture(texturex, UV).rgb,1) * 1.0 * diffuseFactor;
    }
    else {
        diffuseColor = vec4(0, 0, 0, 0);
    }

    fragColor = vec4(ambientFactor.xyz+diffuseColor.xyz,1.0);

    //fragColor = vec4(texture(texturex, UV).rgb,1);

}
