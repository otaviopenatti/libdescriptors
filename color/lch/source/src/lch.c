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
#include "lch.h"

#include "common.h"
#include "image.h"

#define BINS  4
#define SIZE 64

#define NBINS (BINS*BINS*SIZE)

typedef struct _Property {
  int color;
} Property;

typedef struct _VisualFeature {
  ulong *colorH;
  int n;
} VisualFeature;

typedef struct _CompressedVisualFeature {
  uchar *colorH;
  int n;
} CompressedVisualFeature;

Property *AllocPropertyArray(int n)
{
  Property *v=NULL;
  v = (Property *) calloc(n,sizeof(Property));
  if (v==NULL)
    Error(MSG1,"AllocPropertyArray");
  return(v);
}

VisualFeature *CreateVisualFeature(int n)
{
  VisualFeature *vf=NULL;

  vf = (VisualFeature *) calloc(1,sizeof(VisualFeature));
  if (vf != NULL) {
    vf->colorH = AllocULongArray(n);
    vf->n = n;
  } else {
    Error(MSG1,"CreateVisualFeature");
  }
  return(vf);
}

void DestroyVisualFeature(VisualFeature **vf)
{
  VisualFeature *aux;

  aux = *vf;
  if (aux != NULL) {
    if (aux->colorH != NULL) free(aux->colorH);
    free(aux);
    *vf = NULL;
  }
}

CompressedVisualFeature *CreateCompressedVisualFeature(int n)
{
  CompressedVisualFeature *cvf=NULL;

  cvf = (CompressedVisualFeature *) calloc(1,sizeof(CompressedVisualFeature));
  if (cvf != NULL) {
    cvf->colorH = AllocUCharArray(n);
    cvf->n = n;
  } else {
    Error(MSG1,"CreateCompressedVisualFeature");
  }
  return(cvf);
}

void DestroyCompressedVisualFeature(CompressedVisualFeature **cvf)
{
  CompressedVisualFeature *aux;

  aux = *cvf;
  if (aux != NULL) {
    if (aux->colorH != NULL) free(aux->colorH);
    free(aux);
    *cvf = NULL;
  }
}

int *QuantizeColors(CImage *cimg, int color_dim)
{
  ulong i;
  ulong r, g, b;
  ulong fator_g, fator_b;
  int *color, n;
  
  n = cimg->C[0]->nrows * cimg->C[0]->ncols;  

  color = AllocIntArray(n);
  
  fator_g = color_dim;
  fator_b = fator_g*color_dim;
  
  for(i=0; i<n; i++){
    r = color_dim*cimg->C[0]->val[i]/256;
    g = color_dim*cimg->C[1]->val[i]/256;
    b = color_dim*cimg->C[2]->val[i]/256;
    
    color[i] = (r + fator_g*g + fator_b*b);
  }
  return(color);
}

inline uchar ComputeNorm(float value)
{
  return((uchar)(255. * value));
}

void CompressHistogram(uchar *ch, ulong *h, ulong max, int size)
{
  int i;
  uchar v;
  
  for(i=0; i<size; i++){
    v = ComputeNorm((float) h[i] / (float) max);
    ch[i] = (uchar)(v);
  }
}

Property *ComputePixelsProperties(CImage *cimg)
{
  Property *p=NULL;
  int *color, i, n;
  
  n = cimg->C[0]->nrows * cimg->C[0]->ncols;  
  
  p = AllocPropertyArray(n);
  
  color = QuantizeColors(cimg, 4);
  for(i=0; i<n; i++) 
    p[i].color=color[i];
  
  free(color);
  return(p);
}

VisualFeature *ComputeHistograms(Property *p, int rows, int cols)
{
  VisualFeature *vf=NULL;
  ulong fator_x, fator_y;
  ulong x, y;
  ulong r, c;
  ulong i;

  vf = CreateVisualFeature(NBINS);
  for(i=0; i<NBINS; i++){
    vf->colorH[i] = 0;
  }

  fator_x = SIZE;
  fator_y = BINS * SIZE;

  for(r=0; r<rows; r++)
    for(c=0; c<cols; c++) {
      x = BINS*r/rows;
      y = BINS*c/cols;

      vf->colorH[p[r*cols+c].color + fator_x*x + fator_y*y]++;
    }

  return(vf);
}



CompressedVisualFeature *CompressHistograms(VisualFeature *vf, int npixels)
{
  CompressedVisualFeature *cvf=NULL;

  cvf = CreateCompressedVisualFeature(NBINS);
  CompressHistogram(cvf->colorH, vf->colorH, npixels, NBINS);
  
  return(cvf);
}

CompressedVisualFeature *ReadCompressedVisualFeatures(char *filename)
{
  CompressedVisualFeature *cvf=NULL;
  FILE *fp;
  int i, n;
  uchar c;

  fp = fopen(filename,"r");
  if (fp == NULL){
    fprintf(stderr,"Cannot open %s\n",filename);
    exit(-1);
  }
  fscanf(fp,"%d\n",&n);
  cvf = CreateCompressedVisualFeature(n);
  for (i=0; i<n; i++) {
    fscanf(fp,"%c",&c);
    cvf->colorH[i] = c;
  }
  fclose(fp);
  return(cvf);
}

void WriteCompressedVisualFeatures(CompressedVisualFeature *cvf,char *filename)
{
  FILE *fp;
  int i;
  
  fp = fopen(filename,"w");
  if (fp == NULL){
    fprintf(stderr,"Cannot open %s\n",filename);
    exit(-1);
  }
  
  fprintf(fp,"%d\n",NBINS);
  for(i=0;i<NBINS;i++)
    fprintf(fp, "%c", cvf->colorH[i]);
  
  fclose(fp);
}

CompressedVisualFeature *ExtractCompressedVisualFeatures(CImage *cimg)
{
  CompressedVisualFeature *cvf=NULL;
  VisualFeature *vf;
  Property *p;
  int npixels;

  npixels = cimg->C[0]->ncols * cimg->C[0]->nrows;

  p = ComputePixelsProperties(cimg);
  vf = ComputeHistograms(p, cimg->C[0]->nrows, cimg->C[0]->ncols);
  cvf = CompressHistograms(vf, npixels);

  free(p);
  DestroyVisualFeature(&vf);
  return(cvf);
}


Histogram *LCH(CImage *cimg, Image *mask)
{
  Histogram *histogram = NULL;
  CompressedVisualFeature *cvf;
  int i;
  
  cvf = ExtractCompressedVisualFeatures(cimg);

  histogram = CreateHistogram(NBINS);  
  for (i=0; i<NBINS; i++)
    histogram->v[i] = cvf->colorH[i];
  DestroyCompressedVisualFeature(&cvf);

  return(histogram);
}

