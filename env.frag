#version 130
varying vec3 fragment_local_position;
varying vec3 fragment_position;
varying vec3 fragment_normal;

uniform sampler2D env_texture; 

vec4 Environment(vec3 dir)
{
	float PI = 3.14159265358979323846264;
	dir = normalize(dir);
	float r = acos(dir.z)/(PI*sqrt(pow(dir.x,2.0)+pow(dir.y,2.0)));
	float u = (r*dir.x + 1.0)/2.0;
	float v = (r*dir.y + 1.0)/2.0;
    return texture(env_texture, vec2(u, v));
}

void main()
{     
    vec4 texture_color=Environment(fragment_local_position); 
    gl_FragColor = vec4(vec3(texture_color), 1.0); 	
} 
