varying vec4 fragment_position;
varying vec4 fragment_normal;

uniform sampler2D env_texture; 


vec4 Environment(vec3 dir)
{
    return vec4(0.0, 0.0, 0.0, 1.0);
}


vec3 Reflection(vec3 V, vec3 N)
{
    return vec3(0.0, 0.0, 0.0);
}

vec3 Refraction(vec3 V, vec3 N, float index)
{
    return vec3(0.0, 0.0, 0.0);
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
	vec3 N = vec3(fragment_normal);
	L = normalize(L);
	N = normalize(N);
    float brightness=max(abs(dot(L,N)), 0.0);	
    
    //Now you need to calculate reflections and refractions (in the world space)!
    
    

    gl_FragColor = vec4(brightness, brightness, brightness, 1);
 } 
