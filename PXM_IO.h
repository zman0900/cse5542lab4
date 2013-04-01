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
// PXM_IO.h
//**************************************************************************************
#ifndef __FILE_IO_PXM_IO_H__
#define __FILE_IO_PXM_IO_H__
#include	<fstream>


bool PXM_Read(const char *filename, unsigned char **data, int &width, int &height)
{
	FILE *fp = fopen(filename, "rb" );
	if (!fp) {printf("ERROR: failed to read: %s\n", filename); return false;}

	float endian;
	char type[128];
	fscanf(fp, "%s\n", type);
	//Skip the comments
	while(!feof(fp))
	{
		int c=fgetc(fp);
		if(c=='#')
		{
			while(!feof(fp))	
			{
				char c=fgetc(fp);
				if(c=='\r' || c=='\n')	break;
			}
		}
		else	
		{
			ungetc(c, fp);
			break;
		}
	}

	fscanf( fp, "%d %d\n", &width, &height);
	fscanf( fp, "%f\n", &endian);

	unsigned char b[4];

	if(*data) delete *data;
	*data=new unsigned char[width*height*3];
	if(strcmp(type, "P6")==0)
	{
		for(int j=height-1; j>=0; j--)
		for(int i=0; i<width; i++)
		{
			if(feof(fp)) printf("Warning: end of file already.\n");
			(*data)[(j*width+i)*3+0]=fgetc(fp);
			(*data)[(j*width+i)*3+1]=fgetc(fp);
			(*data)[(j*width+i)*3+2]=fgetc(fp);			
		}		
	}
    if(strcmp(type, "PF")==0)
	{
        unsigned char b[4];
        float *f_value=(float *)b;
		for(int j=0; j<height; j++)
            for(int i=0; i<width; i++)
            {
                if(feof(fp)) printf("Warning: end of file already.\n");
                if(endian>0) {b[3]=fgetc(fp); b[2]=fgetc(fp); b[1]=fgetc(fp); b[0]=fgetc(fp);}
                else         {b[0]=fgetc(fp); b[1]=fgetc(fp); b[2]=fgetc(fp); b[3]=fgetc(fp);}
                if(*f_value<0)  *f_value=0;
                if(*f_value>1)  *f_value=1;
                (*data)[(j*width+i)*3+0]=*f_value*255.0;
                if(endian>0) {b[3]=fgetc(fp); b[2]=fgetc(fp); b[1]=fgetc(fp); b[0]=fgetc(fp);}
                else         {b[0]=fgetc(fp); b[1]=fgetc(fp); b[2]=fgetc(fp); b[3]=fgetc(fp);}
                if(*f_value<0)  *f_value=0;
                if(*f_value>1)  *f_value=1;
                (*data)[(j*width+i)*3+1]=*f_value*255.0;
                if(endian>0) {b[3]=fgetc(fp); b[2]=fgetc(fp); b[1]=fgetc(fp); b[0]=fgetc(fp);}
                else         {b[0]=fgetc(fp); b[1]=fgetc(fp); b[2]=fgetc(fp); b[3]=fgetc(fp);}
                if(*f_value<0)  *f_value=0;
                if(*f_value>1)  *f_value=1;
                (*data)[(j*width+i)*3+2]=*f_value*255.0;
		
            }		
	}
	fclose( fp );
	printf("read %s successfully. The resolution is: %d, %d\n", filename, width, height);
	return fp!=NULL;
}


bool PXM_Read(const char *filename, float **data, int &width, int &height)
{
	FILE *fp = fopen(filename, "rb" );
	if (!fp) {printf("ERROR: failed to read: %s\n", filename); return false;}
	
	float endian;
	char type[128];
	fscanf(fp, "%s\n", type);
	//Skip the comments
	while(!feof(fp))
	{
		int c=fgetc(fp);
		if(c=='#')
		{
			while(!feof(fp))	
			{
				char c=fgetc(fp);
				if(c=='\r' || c=='\n')	break;
			}
		}
		else	
		{
			ungetc(c, fp);
			break;
		}
	}
    
	fscanf( fp, "%d %d\n", &width, &height);
	fscanf( fp, "%f\n", &endian);

	unsigned char b[4];
    
	if(*data) delete *data;
	*data=new float[width*height*4];
    if(strcmp(type, "PF")==0)
	{
        unsigned char b[4];
        float *f_value=(float *)b;
		for(int j=0; j<height; j++)
            for(int i=0; i<width; i++)
            {
                if(feof(fp)) printf("Warning: end of file already.\n");
                if(endian>0) {b[3]=fgetc(fp); b[2]=fgetc(fp); b[1]=fgetc(fp); b[0]=fgetc(fp);}
                else         {b[0]=fgetc(fp); b[1]=fgetc(fp); b[2]=fgetc(fp); b[3]=fgetc(fp);}
                (*data)[(j*width+i)*4+0]=*f_value;
                if(endian>0) {b[3]=fgetc(fp); b[2]=fgetc(fp); b[1]=fgetc(fp); b[0]=fgetc(fp);}
                else         {b[0]=fgetc(fp); b[1]=fgetc(fp); b[2]=fgetc(fp); b[3]=fgetc(fp);}
                (*data)[(j*width+i)*4+1]=*f_value;
                if(endian>0) {b[3]=fgetc(fp); b[2]=fgetc(fp); b[1]=fgetc(fp); b[0]=fgetc(fp);}
                else         {b[0]=fgetc(fp); b[1]=fgetc(fp); b[2]=fgetc(fp); b[3]=fgetc(fp);}
                (*data)[(j*width+i)*4+2]=*f_value;
                (*data)[(j*width+i)*4+3]=1;
            }		
	}
	fclose( fp );
	printf("read %s successfully. The resolution is: %d, %d\n", filename, width, height);
	return fp!=NULL;
}



bool PXM_Write(const char *filename, unsigned char *data, int width, int height)
{
	FILE *fp = fopen(filename, "wb");	
	if (!fp) return false;
	
	char type[128];
	fprintf(fp, "P6 ");
	fprintf(fp, "%d %d ", width, height);
	fprintf(fp, "%d ", 255);
	for(int j=height-1; j>=0; j--)
	for(int i=0; i<width; i++)
	{
		fputc(data[(j*width+i)*3+0], fp);
		fputc(data[(j*width+i)*3+1], fp);
		fputc(data[(j*width+i)*3+2], fp);	
	}
	fclose(fp);
	printf("Write %s successfully\n", filename);
	return fp!=NULL;
}

#endif //__FILE_IO_PXM_IO_H__
