varying vec3 fragment_local_position;
varying vec3 fragment_position;
varying vec3 fragment_normal;

uniform sampler2D env_texture; 

vec4 Environment(vec3 dir)
{
    return vec4(0.0, 0.0, 0.0, 1.0);
}

void main()
{     
    vec4 texture_color=Environment(fragment_local_position); 
    gl_FragColor = vec4(vec3(texture_color), 1); 	
} 
