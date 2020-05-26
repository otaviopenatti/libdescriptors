/*
    COPYRIGHT 2007-2012, 2020

    This file is part of Lib Descriptors.

    Lib Descriptors is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Lib Descriptors is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Lib Descriptors. If not, see <http://www.gnu.org/licenses/>.
    
    Otavio A. B. Penatti - otavio_at_penatti_dot_com
    https://github.com/otaviopenatti/libdescriptors
*/
#include "../include/mpeg7htd.h"
#include "../include/common.h"
#include "../include/cimage.h"
#include "../include/matrix.h"
#include "../include/gabor.h"

//this implementation does not use the global standard deviaton from the dataset

float GlobalStandardDesviationMediaHTD() {
	return 1.0;
}
float GlobalStandardDesviationDesvioHTD() {
	return 1.0;
}
/**************************************************************************/

float ComplexMagnitude(float real, float imag) {
	return sqrt(real*real + imag*imag);
}


Mpeg7HTDDescriptor *Mpeg7HTDDescriptorInit(int scale, int orientation) {
	Mpeg7HTDDescriptor *descr = NULL;
	descr = (Mpeg7HTDDescriptor *) malloc(sizeof(Mpeg7HTDDescriptor));
	descr->scale = scale;
	descr->orientation = orientation;
	return descr;
}

Mpeg7HTDDescriptor *CreateMpeg7HTDDescriptorReady(int scale, int orientation, float *media, float *desvio) {
	Mpeg7HTDDescriptor *descr = Mpeg7HTDDescriptorInit(scale,orientation);
	descr->media = media;
	descr->desvio = desvio;
	return descr;
}

Mpeg7HTDDescriptor *CreateMpeg7HTDDescriptor(int hei, int wid, int side, int scale, int orientation, 
		int flag, double Ul, double Uh, Matrix *F_r, Matrix *F_i) {
	
	Mpeg7HTDDescriptor *descr = Mpeg7HTDDescriptorInit(scale,orientation);
	
	float *media  = (float *) malloc(sizeof(float) * scale * orientation);
	float *desvio = (float *) malloc(sizeof(float) * scale * orientation);
	
	int s,o;
	float sum = 0;
	int i = 0;
	int j = 0;
	int h,w;
	int n = hei * wid;
	
	for(s=0,i=0,j=0;s<scale;s++) {
		for(o=0;o<orientation;o++) {
			sum = 0.0;
			for(h=0;h<hei;h++) {
				for(w=0;w<wid;w++) {
					int b = j %  F_r->width;
					int a = (j-b) / F_r->width;
					float v = ComplexMagnitude(F_r->data[a][b],F_i->data[a][b]);					
					sum += v;
					j++;
				}
			}
			media[i] = ((float) sum / (float) n);
			i++;
		}
	}
	for(s=0,i=0,j=0;s<scale;s++) {
		for(o=0;o<orientation;o++) {
			sum = 0.0;
			for(h=0;h<hei;h++) {
				for(w=0;w<wid;w++) {
					int b = j %  F_r->width;
					int a = (j-b) / F_r->width;
					float v = ComplexMagnitude(F_r->data[a][b],F_i->data[a][b]);					
					sum += (v - media[i]) * (v - media[i]);
					j++;
				}
			}
			desvio[i] = sqrt((float) sum / (float) n);
			i++;
		}
	}
	descr->media = media;
	descr->desvio = desvio;
	
	return descr;	
}
void DestroyMpeg7HTDDescriptor(Mpeg7HTDDescriptor **c) {
	Mpeg7HTDDescriptor *aux;

	aux = *c;
	if (aux != NULL) {
		if(aux->media!=NULL)free(aux->media);
		if(aux->desvio!=NULL)free(aux->desvio);
		free(aux);
		*c = NULL;
	}	
}

Mpeg7HTDDescriptor *ExtractMpeg7HTDDescriptor(CImage *rgbImage) {
	int hei, wid, i, j, side, scale, orientation, flag;
	Matrix *img, *F_r, *F_i;	
	double Ul, Uh;
	
	wid = rgbImage->C[0]->ncols;
	hei = rgbImage->C[0]->nrows;
	CreateMatrix(&img, hei, wid);
	
	//converts to gray-scale
	for (i=0;i<hei;i++)
		for (j=0;j<wid;j++)  {
			int p = i*wid+j;
			int R = rgbImage->C[0]->val[p];
			int G = rgbImage->C[1]->val[p];
			int B = rgbImage->C[2]->val[p];
			//according to http://www.mathworks.com/support/solutions/data/1-1ASCU.html
			double intensity = ((double)0.2989 * R + 0.5870 * G + 0.1140 * B);
			//rounding (truncating!)
			img->data[i][j] =(int)(intensity);		
		}
	
	//parameters according to PAMI paper
	scale = 4;
	orientation = 6;
	Ul = 0.05;
	Uh = 0.4;
	flag = 0;
	side = MIN(128, MIN(hei,wid));
	
	//creates the filtered images for all orientations and scales
	CreateMatrix(&F_r, hei*scale, wid*orientation);
	CreateMatrix(&F_i, hei*scale, wid*orientation);
	GaborFilteredImg(F_r, F_i, img, side, Ul, Uh, scale, orientation, flag);
	
	//creates the descritor
	Mpeg7HTDDescriptor *descr = CreateMpeg7HTDDescriptor(hei,wid,side, scale, orientation, flag, Ul, Uh, F_r, F_i);	

	FreeMatrix(F_r);
	FreeMatrix(F_i);
	FreeMatrix(img);
	return descr;
}


void WriteStreamMpeg7HTDDescriptor(Mpeg7HTDDescriptor *c, FILE *fp) {
	if(c == NULL) return;
	if (fp != NULL) {
		float *media = c->media;
		float *desvio = c->desvio;
		fwrite(&(c->scale),sizeof(int),1,fp);
		fwrite(&(c->orientation),sizeof(int),1,fp);
		int n = c->scale * c->orientation;
		fwrite(media,sizeof(float),n,fp);
		fwrite(desvio,sizeof(float),n,fp);
	}
}
void WriteFileMpeg7HTDDescriptor(Mpeg7HTDDescriptor *c, char *filename) {
	FILE *fp;
	if(c == NULL) return;

	fp = fopen(filename, "wb");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open %s\n", filename);
		exit(-1);
	}
	WriteStreamMpeg7HTDDescriptor(c, fp);
	fclose(fp);
}

Mpeg7HTDDescriptor *ReadStreamMpeg7HTDDescriptor(FILE *fp) {
	Mpeg7HTDDescriptor *descr = NULL; 
	if (fp != NULL) {
		int scale,orientation;
		fread(&scale,sizeof(int),1,fp);
		fread(&orientation,sizeof(int),1,fp);
		int n = scale * orientation;
		float *media = (float *) calloc(sizeof(float),  scale * orientation);
		float *desvio = (float *) calloc(sizeof(float),  scale * orientation);
		assert(media!=NULL);
		assert(desvio!=NULL);
		fread(media,sizeof(float),n,fp);
		fread(desvio,sizeof(float),n,fp);
		descr = CreateMpeg7HTDDescriptorReady(scale, orientation, media, desvio);			
	}
	return descr;
}

Mpeg7HTDDescriptor *ReadFileMpeg7HTDDescriptor(char *fv_path) {
	FILE *fp;
	fp = fopen(fv_path, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open %s\n", fv_path);
		exit(-1);
	}
	Mpeg7HTDDescriptor *c = ReadStreamMpeg7HTDDescriptor(fp);
	fclose(fp);
	return c;
}
double DistanceMpeg7HTDDescriptor(Mpeg7HTDDescriptor *descr1, Mpeg7HTDDescriptor *descr2) {
	int s,o;
	int scale = descr1->scale;
	int orientation = descr1->orientation;
	
	assert(descr2->orientation == orientation);
	assert(descr2->scale == scale);
	
	int i = 0;
	float globalDesvMedia  = GlobalStandardDesviationMediaHTD();
	float globalDesvDesvio = GlobalStandardDesviationDesvioHTD();
	float d = 0.0;
	for(s=0;s<scale;s++) {
		for(o=0;o<orientation;o++) {
			float m1 = descr1->media[i];
			float m2 = descr2->media[i];
			float d1 = descr1->desvio[i];
			float d2 = descr2->desvio[i];
			d += fabs((m1-m2)/globalDesvMedia) + fabs((d1-d2) / globalDesvDesvio);
			i++;
		}
	}
	return d;
}
