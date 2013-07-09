/* [FRAG] */ 
#version 330

in vec2 UV;
in vec3 normal;

out vec4 fragColor;

uniform sampler2D texturex;
uniform mat4 view;

void main()
{

    vec4 textureColor = vec4(texture(texturex, UV).rgb,1);

    vec3 lightDirection = normalize(vec3(0,0,1));
    float ambientIntensity = 0.2;
    vec4 diffuseLightColor = vec4(1.0);

    vec4 ambientColor = textureColor*ambientIntensity;
    float diffuseFactor = dot(normal, -lightDirection);

    vec4 diffuseColor;
    if (diffuseFactor > 0) {
                diffuseColor = textureColor * 1.0 * diffuseFactor;
    }
    else {
        diffuseColor = vec4(0, 0, 0, 0);
    }

    float cosAngIncidence = dot(normal, lightDirection);
    cosAngIncidence = clamp(cosAngIncidence, 0, 1);

    vec3 viewDirection = normalize(-(vec3(view[3])));
    vec3 halfAngle = normalize(lightDirection + viewDirection);
    float blinnTerm = dot(normal, halfAngle);

    blinnTerm = clamp(blinnTerm, 0, 1);
    blinnTerm = cosAngIncidence != 0.0 ? blinnTerm : 0.0;
    blinnTerm = pow(blinnTerm, 0.9);

    vec4 specularColor = textureColor*blinnTerm;

    fragColor = vec4(ambientColor.xyz+diffuseColor.xyz+specularColor.xyz,1.0);
}
