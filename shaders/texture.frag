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

    //currently implementing lighting in world coordinates
    vec3 lightDirection = normalize(vec3(0,-1,-1));  //world coordinates
    float ambientIntensity = 0.2;
    vec4 diffuseLightColor = vec4(1.0);

    vec4 ambientColor = textureColor*ambientIntensity;
    float diffuseFactor = clamp(dot(normalize(normal), -lightDirection),0.0,1.0);

    vec4 diffuseColor;
    if (diffuseFactor > 0) {
                diffuseColor = textureColor * 1.0 * diffuseFactor;
    }
    else {
        diffuseColor = vec4(0, 0, 0, 0);
    }

    vec3 reflection = reflect(lightDirection,normalize(normal));
    float specularFactor = clamp(dot(normalize(reflection),normalize(normal)),0.0,1.0);

    //kinda hacky but oh well
    vec4 specularColor = pow(specularFactor,128) * textureColor;

    fragColor = vec4(ambientColor.xyz+diffuseColor.xyz+specularColor.xyz,1.0);
}
