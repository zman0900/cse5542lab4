#version 130
attribute vec4 vertex_position;     //In the local space
attribute vec3 vertex_normal;       //In the local space

varying vec3 fragment_local_position;
varying vec3 fragment_position;
varying vec3 fragment_normal;

void main()
{ 
    fragment_local_position = vec3(vertex_position);                //In the local space
	fragment_position = vec3(gl_ModelViewMatrix * vertex_position); //In the eye space
	fragment_normal   = gl_NormalMatrix * vertex_normal;   //In the eye space
	gl_Position = gl_ModelViewProjectionMatrix * vertex_position;   //In the clip space
}
