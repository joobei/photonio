/* FRAGMENT */

#version 330

uniform vec3 view;

layout (std140) uniform Material {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
	int texCount;
};

uniform	sampler2D texUnit;
uniform	sampler2D bumpMap;
   
in vec3 Normal;
in vec2 TexCoord;
out vec4 output;
float lightAttenuation = 0.5;
shininessFactor :: shininess;
specularColor :: specular;
cameraSpacePosition :: view;

float CalcAttenuation(in vec3 cameraSpacePosition, out vec3 lightDirection)
{
	vec3 lightDifference =  view - cameraSpacePosition;
	float lightDistanceSqr = dot(lightDifference, lightDifference);
	lightDirection = lightDifference * inversesqrt(lightDistanceSqr);
	
	return (1 / ( 1.0 + lightAttenuation * sqrt(lightDistanceSqr)));
}

void main()
{

	vec3 lightDir = vec3(0.0);
	float atten = CalcAttenuation(cameraSpacePosition, lightDir);
	vec4 attenIntensity = atten * lightIntensity;
	
	vec3 surfaceNormal = normalize(vertexNormal);
	float cosAngIncidence = dot(surfaceNormal, lightDir);
	cosAngIncidence = clamp(cosAngIncidence, 0, 1);
	
	vec3 viewDirection = normalize(-cameraSpacePosition);
	
	vec3 halfAngle = normalize(lightDir + viewDirection);
	float blinnTerm = dot(surfaceNormal, halfAngle);
	blinnTerm = clamp(blinnTerm, 0, 1);
	blinnTerm = cosAngIncidence != 0.0 ? blinnTerm : 0.0;
	blinnTerm = pow(blinnTerm, shininessFactor);

	outputColor = (texture2D(texUnit, TexCoord) * attenIntensity * cosAngIncidence) +
		(specularColor * attenIntensity * blinnTerm) +
		(diffuseColor * ambientIntensity);
}