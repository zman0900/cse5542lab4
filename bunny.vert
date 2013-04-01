attribute vec4 vertex_position;     //In the local space
attribute vec3 vertex_normal;       //In the local space

varying vec4 fragment_position;
varying vec3 fragment_normal;

void main()
{ 
	fragment_position = (gl_ModelViewMatrix * vertex_position); //In the eye space
	fragment_normal   = vec3(gl_ModelViewMatrix * vec4(vertex_normal, 0.0));   //In the eye space
	gl_Position = gl_ModelViewProjectionMatrix * vertex_position;   //In the clip space
}
