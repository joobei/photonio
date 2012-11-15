/* [FRAG] */
#version 330

in vec4 ex_Color;
in vec3 ex_Normal;

out vec4 fragColor;

void main()
{
	vec3 normalize(lightDirection = vec3(1,-1 ,0));
	float ambientIntensity = 0.8;
	vec4 diffuseLightColor = vec4(1.0);

	vec4 diffuseColor;

	

	vec4 ambientColor = ex_Color*ambientIntensity;

	float diffuseFactor = dot(normalize(ex_Normal), -lightDirection);

    if (diffuseFactor > 0) {

        diffuseColor =  * 1.0 * diffuseFactor;
    }
    else {
        diffuseColor = vec4(0, 0, 0, 0);
    }

    fragColor = vec4(ambientColor.xyz+diffuseColor.xyz,1.0);
	//fragColor = vec4(ambientColor.xyz,1.0);
}
