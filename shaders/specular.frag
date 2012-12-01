/* [FRAG] */
#version 330

in vec4 color;
in vec3 normal;

out vec4 fragColor;

uniform float alpha;

void main()
{
	vec3 lightDirection = normalize(vec3(1,0,0));
	float ambientIntensity = 0.5;
	vec4 diffuseLightColor = vec4(1.0);

	vec4 diffuseColor;

	vec4 ambientColor = color*ambientIntensity;

	float diffuseFactor = dot(normal, -lightDirection);

    if (diffuseFactor > 0) {
        
		diffuseColor =  color * 1.0 * diffuseFactor;
    }
    else {
        diffuseColor = vec4(0, 0, 0, 0);
    }
	
	fragColor = vec4(ambientColor.xyz,alpha)+vec4(diffuseColor.xyz,alpha);
}
