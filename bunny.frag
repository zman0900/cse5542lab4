#version 130
varying vec4 fragment_position;
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


vec3 Reflection(vec3 V, vec3 N)
{
    return 2.0 * dot(V, N) * N - V;
}

vec3 Refraction(vec3 V, vec3 N, float index)
{
	vec3 S = (1.0/index)*(V-dot(V,N)*N);
	return -1.0 * sqrt(1.0 - dot(S,S))*N - S;
}

float Fresnel(vec3 V, vec3 N, float index)
{
    return 0.0;
}

void main()
{   
    //Calculate illumination in the eye space
    vec3 light_source=vec3(0, 0, 0);
    vec3 V = -vec3(fragment_position);
	vec3 L = light_source - vec3(fragment_position);
	L = normalize(L);
	vec3 N = normalize(fragment_normal);
    float brightness=max(abs(dot(L,N)), 0.0);	
    
    //Now you need to calculate reflections and refractions (in the world space)!
    V = normalize(V);
    vec4 cr = Environment(vec3(vec4(Reflection(V, N), 1.0)
                                                  * gl_ModelViewMatrixInverse));
    vec4 ct = Environment(vec3(vec4(Refraction(V, N, 1.5), 1.0)
                                                  * gl_ModelViewMatrixInverse));

    gl_FragColor = vec4(brightness * vec3(ct), 1.0);
 } 
