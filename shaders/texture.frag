/* [FRAG] */ 
#version 330

in vec2 UV;
in vec3 normal;
in vec4 cameraSpacePosition;

out vec4 fragColor;

uniform sampler2D texturex;

void main()
{

    vec4 textureColor = vec4(texture(texturex, UV).rgb,1);

    vec3 lightDirection = normalize(vec3(0,-1,-1));
    float ambientIntensity = 0.2;
    vec4 diffuseLightColor = vec4(1.0);

    vec4 ambientColor = textureColor*ambientIntensity;
    float diffuseFactor = dot(normalize(normal), -lightDirection);

    vec4 diffuseColor;
    if (diffuseFactor > 0) {
                diffuseColor = textureColor * 1.0 * diffuseFactor;
    }
    else {
        diffuseColor = vec4(0, 0, 0, 0);
    }

    vec3 reflection = reflect(lightDirection,normalize(normal));
    float specularFactor = dot(normalize(reflection),normalize(normal));

    //kinda hacky but oh well
    vec4 specularColor = pow(specularFactor,64) * textureColor;

    fragColor = vec4(ambientColor.xyz+diffuseColor.xyz+specularColor.xyz,1.0);
}
