#version 330

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;
uniform mat3 u_normalMatrix;

uniform mat4 u_shadowMatrix;

in vec4 in_Vertex;
in vec3 in_Normal;
in vec2 in_TexCoord;

out vec3 v_normal;
out vec3 v_eye;
out vec4 v_projCoord;
out vec2 texCoord;

void main(void)
{
	vec4 vertex = u_modelMatrix*in_Vertex;

	// Bring the vertex to the light / shadow coordinate system
	v_projCoord = u_shadowMatrix*vertex;

	vertex = u_viewMatrix*vertex;

	v_eye = -vec3(vertex);

	v_normal = u_normalMatrix*in_Normal;

	gl_Position = u_projectionMatrix*vertex;
}