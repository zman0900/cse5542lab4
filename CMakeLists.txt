cmake_minimum_required (VERSION 2.6)
project (cse5542lab4)
add_executable (lab4 main.cpp MESH.h MY_GLSL.h PXM_IO.h)
target_link_libraries (lab4 glut GL GLU GLEW)
