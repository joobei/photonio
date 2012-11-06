/* [FRAG] */
#version 330 /* Specify which version of GLSL we are using. */ 
uniform vec4 baseColor; 

//flat in vec4 pos;

out vec4 fragColor;


void main() 
{ 
	/* Calculate distance to circle center
    float d = distance(pos.xy, vec2(0.3,0.3));

    // Discard fragments regarding the distance to the center.
    if( d > 0.5 || d < 0.45)
        discard;
	*/

    gl_FragColor = baseColor;
} 