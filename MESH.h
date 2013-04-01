//**************************************************************************************
//  Copyright (C) 2002 - 2011, Huamin Wang
//  All rights reserved.
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//     1. Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//     2. Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//     3. The names of its contributors may not be used to endorse or promote
//        products derived from this software without specific prior written
//        permission.
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//**************************************************************************************
// MESH.h
//**************************************************************************************
#ifndef __WHMIN_MESH_H__
#define __WHMIN_MESH_H__
#include	<fstream>
#include    <math.h>
#include <cstring>

float Normalize(float *x, float *r=0)
{
	if(r==0) r=x;
	float m=sqrtf(x[0]*x[0]+x[1]*x[1]+x[2]*x[2]);
	if(m<1e-14f) return m;
	float inv_m=1/m;
	r[0]=x[0]*inv_m;
	r[1]=x[1]*inv_m;
	r[2]=x[2]*inv_m;
	return m;
}

void Cross(float x[], float y[], float r[])
{
	r[0]= x[1]*y[2]-y[1]*x[2];
	r[1]=-x[0]*y[2]+y[0]*x[2];
	r[2]= x[0]*y[1]-y[0]*x[1];
}


typedef struct 
{
    float p[4];     //Vertex Position
    float n[4];     //Vertex Color
} VERTEX;


class MESH
{
public:
	int			max_number;

	//vertex data
	int			number;		//vertex number
	VERTEX*		X;

	//triangle data
	int			t_number;	//triangle number
	GLuint*     T;			//triangle indices

    GLuint vertex_handle;   
    GLuint triangle_handle;

	MESH()
	{
		max_number	= 100000;
		number		= 0;
		t_number	= 0;
		X			= new VERTEX	[max_number];		
		T			= new GLuint	[max_number*3];
	}

	~MESH()
	{
		if(X)	delete []X;
		if(T)	delete []T;
	}

	void Read_OBJ_File(const char *filename)
	{
		number=0;
		t_number=0;
		int vertex_normal_number=0;
		FILE *fp=fopen(filename, "r+");		
		while(feof(fp)==0)
		{
			char token[1024];
			fscanf(fp, "%s", &token);
			if(token[0]=='#' && token[1]=='\0')
			{
				int c;
				while(feof(fp)==0)
					if((c=fgetc(fp))=='\r' || c=='\n')	break;
			}
			else if(token[0]=='v' && token[1]=='\0')	//vertex
			{
				fscanf(fp, "%f %f %f\n", &X[number].p[0], &X[number].p[1], &X[number].p[2]); 
				X[number].p[3]=1;
				number++;
			}
			else if(token[0]=='v' && token[1]=='t')
			{
				float temp[2];
				fscanf(fp, "%f %f\n", &temp[0], &temp[1]);
			}
			else if(token[0]=='v' && token[1]=='n')
			{				
				vertex_normal_number++;
			}
			else if(token[0]=='f' && token[1]=='\0')
			{
				int data[64];
				int data_number=0;
				memset(data, 0, sizeof(int)*64);
				
				while(feof(fp)==0)
				{
					fscanf(fp, "%d", &data[data_number++]);
					bool end=false;
					while(feof(fp)==0)
					{
						char token=fgetc(fp);
						if(token==' ' || token=='\t')	break;
						if(token=='\n'|| token=='\r')	{end=true; break;}
					}
					if(end)	break;
				}
				for(int i=1; i<data_number-1; i++)
				{
					T[t_number*3+0]=data[0  ]-1;
					T[t_number*3+1]=data[i  ]-1;
					T[t_number*3+2]=data[i+1]-1;
					t_number++;
				}
			}			
		}
		fclose(fp);

        Build_Normal();
	}

    void Create_Sphere(float radius, int m, int n)
    {
        number=0;        
        for(int i=0; i<=m; i++)
            for(int j=0; j<=n; j++)
            {
                float theta=((i  )*2*3.14159)/m;
                float phi  =((j  )  *3.14159)/n-3.14159/2;
                X[number].p[0]=radius*cosf(phi)*cosf(theta);
                X[number].p[1]=radius*cosf(phi)*sinf(theta);
                X[number].p[2]=radius*sinf(phi);
                X[number].p[3]=1;
                number++;
            }
        
        t_number=m*n*2;
        int p=0;
        for(int i=0; i<m; i++)
            for(int j=0; j<n; j++)
            {
                T[p++]=(i  )*(n+1)+j;
                T[p++]=(i  )*(n+1)+j+1;
                T[p++]=((i+1)%m)*(n+1)+j+1;            
                T[p++]=(i  )*(n+1)+j;
                T[p++]=((i+1)%m)*(n+1)+j+1;
                T[p++]=((i+1)%m)*(n+1)+j; 
            }
        
        Build_Normal();
    }
    
    void Create_Cylinder(float radius, float height, int m, int n)
    {
        number=0;        
        for(int i=0; i<=m; i++)
            for(int j=0; j<=n; j++)
            {
                float theta=((i  )*2*3.14159)/m;
                float h    =j*height/n-height/2.0;
                X[number].p[0]=radius*cosf(theta);
                X[number].p[2]=radius*sinf(theta);
                X[number].p[1]=h;
                X[number].p[3]=1;
                number++;
            }
        
        t_number=m*n*2;
        int p=0;
        for(int i=0; i<m; i++)
            for(int j=0; j<n; j++)
            {
                T[p++]=(i  )*(n+1)+j;
                T[p++]=(i  )*(n+1)+j+1;
                T[p++]=((i+1)%m)*(n+1)+j+1;            
                T[p++]=(i  )*(n+1)+j;
                T[p++]=((i+1)%m)*(n+1)+j+1;
                T[p++]=((i+1)%m)*(n+1)+j; 
            }
        
        Build_Normal();
    }
    
    void Build_Normal()
    {
		float* TN=new float[max_number*3];
        
		memset(TN, 0, sizeof(float)*number*3);
		for(int i=0; i<t_number; i++)
		{
			float *p0=X[T[i*3+0]].p;
			float *p1=X[T[i*3+1]].p;
			float *p2=X[T[i*3+2]].p;
            
			float e0[3], e1[3];
			for(int j=0; j<3; j++)
			{
				e0[j]=p1[j]-p0[j];
				e1[j]=p2[j]-p0[j];
			}		
			Cross(e1, e0, &TN[i*3]);
			Normalize(&TN[i*3]);
		}
        
		for(int i=0; i<number; i++)
			X[i].n[0]=X[i].n[1]=X[i].n[2]=0;
        
		for(int i=0; i<t_number; i++)
		{
			int v0=T[i*3+0];
			int v1=T[i*3+1];
			int v2=T[i*3+2];
			X[v0].n[0]+=TN[i*3+0];
			X[v0].n[1]+=TN[i*3+1];
			X[v0].n[2]+=TN[i*3+2];
			X[v1].n[0]+=TN[i*3+0];
			X[v1].n[1]+=TN[i*3+1];
			X[v1].n[2]+=TN[i*3+2];
			X[v2].n[0]+=TN[i*3+0];
			X[v2].n[1]+=TN[i*3+1];
			X[v2].n[2]+=TN[i*3+2];
		}
		for(int i=0; i<number; i++)	Normalize(X[i].n);
        
		delete[] TN;        
    }
    
	void Scale(float s)
	{
		for(int i=0; i<number; i++)
		{
			X[i].p[0]*=s;
			X[i].p[1]*=s;
			X[i].p[2]*=s;
		}
	}

	void Centerize()
	{
		//Make sure that the bunny is centered at the origin.
		float center[3];
		center[0]=center[1]=center[2]=0;
		for(int i=0; i<number; i++)
		{
			center[0]+=X[i].p[0];
			center[1]+=X[i].p[1];
			center[2]+=X[i].p[2];
		}
		center[0]/=number;
		center[1]/=number;
		center[2]/=number;
		for(int i=0; i<number; i++)
		{
			X[i].p[0]-=center[0];
			X[i].p[1]-=center[1];
			X[i].p[2]-=center[2];
		}
	}
	
	void Init()
	{
		glGenBuffers(1, &vertex_handle);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_handle);    
        glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX)*number, X, GL_STATIC_DRAW);     
        
        glGenBuffers(1, &triangle_handle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_handle); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*t_number*3, T, GL_STATIC_DRAW);
	}
};

#endif
