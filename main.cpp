
////////////////////////////////////////////////////////////////////////////////////////////////
// 
// The purpose of this program is to use vertex buffer objects (VBOs) to define a simple 3D cube
// Note that we will not use glBegin/End any more since they have been deprecated in OpenGL 3.x 
// 
// A very simple shader (cubeDxform.{vert,frag}) is uesd in this example
//
// In this example, the triangles vertices and colors are listed in separate arrays and VBOs. We also 
// use a triangle index array (GL_ELEMENT_ARRAY_BUFFER) to assemble triangles 
// vertices. As a result, we use glDrawArrays() to draw. 
//
// Simple 3D viewing is set up, using OpenGL 2.x convention. That is, we are not manipulating 
// our own transformation matrix, and still relying on the fixed function pipeline to accumulate 
// the matrices (GL_MODELVIEW, GL_PROJECTIon) for us. 
//
// Han-Wei Shen, July 2012 @ Ohio State University 
// hwshen@cse.ohio-state.edu 
// modified by Huamin Wang, Feb 2013 @ Ohio State University
// whmin@cse.ohio-state.edu
////////////////////////////////////////////////////////////////////////////////////////////////
#include <cstddef>
#include "MY_GLSL.h"
#include "MESH.h"
#include "PXM_IO.h"

GLuint env_program;
GLuint bunny_program;

MESH	env_mesh;
MESH    bunny_mesh;

GLuint  texture_id;

void Load_Texture(GLuint& tid, const char* filename)
{
    int             width;
    int             height;
    unsigned char*  data=0;
    //Step 1: Read the image from disk to main memory
    PXM_Read(filename, &data, width, height);
    //Step 2: Generate a texture handle and bind it
    glGenTextures(1, &tid); 
    glBindTexture(GL_TEXTURE_2D, tid);
    //Step 3: Set the texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //Step 4: Associate the image data with the handle and send it to GPU memory
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    //Step 5: We can delete the image data in the main memory now.
    delete[] data;
}


void Load_HDR_Texture(GLuint& tid, const char* filename)
{
    int             width;
    int             height;
    float*          data=0;
    //Step 1: Read the image from disk to main memory
    PXM_Read(filename, &data, width, height);
    //Step 2: Generate a texture handle and bind it
    glGenTextures(1, &tid); 
    glBindTexture(GL_TEXTURE_2D, tid);
    //Step 3: Set the texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Step 4: Associate the image data with the handle and send it to GPU memory
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, width, height, 0, GL_RGBA, GL_FLOAT, data);
    //Step 5: We can delete the image data in the main memory now.
    delete[] data;
}


//**************************************************************************************
// GLUT motion variables
//**************************************************************************************
#define NO_MOTION			0
#define ZOOM_MOTION			1
#define ROTATE_MOTION		2
#define TRANSLATE_MOTION	3
#define	SELECT_MOTION		4

int   motion_mode=NO_MOTION;
int   mouse_x, mouse_y; 
float swing_angle   = 0.0; 
float elevate_angle = 0.0; 
float zoom = 2; 



//**************************************************************************************
// Display
//**************************************************************************************
void display() 
{ 
	glEnable(GL_DEPTH_TEST);    // need depth test to correctly draw 3D objects 
	glClearColor(0,0,0,1); 
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

	// Now we are ready to draw, using the triangle indices in the buffer 
	// The following are OpenGL2.x convention. In OpenGL 3.x, we 
	// are supposed to manage our own matrices, instead of using GL_MODELVIEW and GL_PROJECTION
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	gluPerspective(90, 1, .1, 10000);

	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 
	gluLookAt(0,0,5,0,0,0,0,1,0); 

    gluLookAt(0, 0, zoom, 0, 0, 0, 0, 1, 0);
    glRotatef(elevate_angle, 1, 0, 0);
    glRotatef(swing_angle, 0, 1, 0);


	glUseProgram(env_program);
    //Set up the texture
    GLuint uniloc = glGetUniformLocation(env_program, "env_texture");
    glUniform1i(uniloc, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    //Set up the mesh
	GLuint c0=glGetAttribLocation(env_program, "vertex_position");
	GLuint c1=glGetAttribLocation(env_program, "vertex_normal");
	glEnableVertexAttribArray(c0);
	glEnableVertexAttribArray(c1); 
	glBindBuffer(GL_ARRAY_BUFFER, env_mesh.vertex_handle); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, env_mesh.triangle_handle);
	// Tells OpenGL how to walk through the VBOs, i.e., how the data are packed 
	// number of coordinates per vertex (4 here), type of the coordinates, 
	// stride between consecutive vertices, and pointers to the first coordinate
	glVertexAttribPointer(c0,4,GL_FLOAT, GL_FALSE, sizeof(VERTEX), reinterpret_cast<void*>(offsetof(VERTEX, p))); 
	glVertexAttribPointer(c1,3,GL_FLOAT, GL_FALSE, sizeof(VERTEX), reinterpret_cast<void*>(offsetof(VERTEX, n)));
	glDrawElements(GL_TRIANGLES, env_mesh.t_number*3, GL_UNSIGNED_INT, (char*) NULL+0); 

    
	glUseProgram(bunny_program);
    //Set up the texture
    uniloc = glGetUniformLocation(bunny_program, "env_texture");
    glUniform1i(uniloc, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    //Set up the mesh
    c0=glGetAttribLocation(bunny_program, "vertex_position");
    c1=glGetAttribLocation(bunny_program, "vertex_normal");
	glEnableVertexAttribArray(c0);
	glEnableVertexAttribArray(c1); 
	glBindBuffer(GL_ARRAY_BUFFER, bunny_mesh.vertex_handle); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunny_mesh.triangle_handle);
	// Tells OpenGL how to walk through the VBOs, i.e., how the data are packed 
	// number of coordinates per vertex (4 here), type of the coordinates, 
	// stride between consecutive vertices, and pointers to the first coordinate
	glVertexAttribPointer(c0,4,GL_FLOAT, GL_FALSE, sizeof(VERTEX), reinterpret_cast<void*>(offsetof(VERTEX, p))); 
	glVertexAttribPointer(c1,3,GL_FLOAT, GL_FALSE, sizeof(VERTEX), reinterpret_cast<void*>(offsetof(VERTEX, n)));
	glDrawElements(GL_TRIANGLES, bunny_mesh.t_number*3, GL_UNSIGNED_INT, (char*) NULL+0);
    
    
	glutSwapBuffers();
} 

//**************************************************************************************
// GLUT user interactions
//**************************************************************************************
void mymouse(int button, int state, int x, int y)
{
    if(state==GLUT_UP) motion_mode=NO_MOTION;
    else
    {
        motion_mode = ROTATE_MOTION;
        mouse_x=x;
        mouse_y=y;			
    }
    glutPostRedisplay();
}

void mymotion(int x, int y)
{
    if( motion_mode!=NO_MOTION)
    {
        if(motion_mode==ROTATE_MOTION) 
        {
            swing_angle   += (float)(x - mouse_x)*360/(float)800;
            elevate_angle += (float)(y - mouse_y)*180/(float)800;
            if     (elevate_angle> 90)	elevate_angle =  90;
            else if(elevate_angle<-90)	elevate_angle = -90;
        }
        mouse_x=x;
        mouse_y=y;
        glutPostRedisplay();
    } 
}

void mykey(unsigned char key, int x, int y)
{
    
    if(key=='a')    zoom/=0.8f;
    if(key=='z')    zoom*=0.8f;    
    
	glutPostRedisplay();
}

//**************************************************************************************
// main
//**************************************************************************************
int main(int argc, char** argv) 
{ 
    
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH); 
	glutInitWindowSize(800, 800); 

	glutCreateWindow("env texture"); 
	glutDisplayFunc(display); 
	glutMouseFunc(mymouse); 
	glutMotionFunc(mymotion);
	glutKeyboardFunc(mykey);

	// initialize GLEW 
	GLenum err = glewInit(); 
	if ( err != GLEW_OK)  printf(" Error initializing GLEW! \n"); 
	else printf("Initializing GLEW succeeded!\n");
    

	env_program   = Setup_GLSL("env");  
	bunny_program = Setup_GLSL("bunny");  

	bunny_mesh.Read_OBJ_File("bunny.obj");
    bunny_mesh.Scale(20);
    bunny_mesh.Centerize();
   // bunny_mesh.Create_Sphere(3, 40, 40);
    bunny_mesh.Init();
    
    env_mesh.Create_Sphere(200, 100, 100);
	env_mesh.Init();

    Load_HDR_Texture(texture_id, "stpeters_probe.pfm");
	    
	glutMainLoop(); 
} 
