attribute vec4 vertex_position;     //In the local space
attribute vec4 vertex_normal;       //In the local space

varying vec4 fragment_position;
varying vec4 fragment_normal;

void main()
{ 
	fragment_position = (gl_ModelViewMatrix * vertex_position); //In the eye space
	fragment_normal   = (gl_ModelViewMatrix * vertex_normal);   //In the eye space
	gl_Position = gl_ModelViewProjectionMatrix * vertex_position;   //In the clip space
}
