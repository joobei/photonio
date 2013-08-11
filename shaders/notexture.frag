/* [FRAG] */ 
#version 330
precision highp float;

uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;

uniform vec4 light_diffuse;
uniform vec4 light_specular;

in vec2 Vertex_UV;
in vec3 Vertex_Normal;
in vec4 Vertex_LightDir;
in vec4 Vertex_EyeVec;
out vec4 Out_Color;

void main()
{
    vec2 uv = Vertex_UV.xy;

    vec3 N = normalize(Vertex_Normal.xyz);
    vec3 L = normalize(Vertex_LightDir.xyz);
    vec3 V = normalize(Vertex_EyeVec.xyz);
    vec3 PN = N;

    vec4 tex01_color = material_diffuse;
    vec4 final_color = vec4(0.4* tex01_color.rgb,1*tex01_color.a); //ambient light

    float lambertTerm = dot(PN, L);
    if (lambertTerm > 0.0)
    {
        final_color += light_diffuse * vec4(1) * lambertTerm * tex01_color;

        vec3 E = normalize(Vertex_EyeVec.xyz);
        vec3 R = reflect(-L, PN);
        float specular = pow( max(dot(R, E), 0.0), material_shininess);
        final_color += light_specular * vec4(1)/*in place of material_specular*/ * specular;
    }
    Out_Color.rgb = final_color.rgb;
    Out_Color.a = material_diffuse.w;

}
