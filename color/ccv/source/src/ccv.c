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
#include "ccv.h"

#include "adjacency.h"
#include "common.h"
#include "image.h"
#include "queue.h"

#define MIN_AREA 1

#define LOW  0
#define HIGH 1

#define SIZE 64

typedef struct _Property {
  int color;
  int frequency;
} Property;

typedef struct _VisualFeature {
  ulong *lowH;
  ulong *highH;
  int n;
} VisualFeature;

typedef struct _CompressedVisualFeature {
  uchar *lowH;
  uchar *highH;
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
    vf->lowH = AllocULongArray(n);
    vf->highH = AllocULongArray(n);
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
    if (aux->lowH != NULL) free(aux->lowH);
    if (aux->highH != NULL) free(aux->highH);
    free(aux);
    *vf = NULL;
  }
}

CompressedVisualFeature *CreateCompressedVisualFeature(int n)
{
  CompressedVisualFeature *cvf=NULL;

  cvf = (CompressedVisualFeature *) calloc(1,sizeof(CompressedVisualFeature));
  if (cvf != NULL) {
    cvf->lowH = AllocUCharArray(n);
    cvf->highH = AllocUCharArray(n);
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
    if (aux->lowH != NULL) free(aux->lowH);
    if (aux->highH != NULL) free(aux->highH);
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

void ComputeFrequencyProperty(Image *img, Property *ppt, int npixels)
{ 
  int i, x, y, p, q;
  Image *label;
  int nlabels;
  ulong *area;
  Pixel u, v;
  AdjRel *A;
  Queue *Q;
  
  label = CreateImage(img->ncols, img->nrows);
  for (p=0;p<npixels;p++)
    label->val[p] = NIL;

  A = Circular(1.0);

  Q = CreateQueue(SIZE, npixels);
  nlabels = 0;
  for (y=0; y<img->nrows; y++)
    for (x=0; x<img->ncols; x++) {
      p = x + img->tbrow[y];
      if (label->val[p] == NIL) {
        label->val[p] = nlabels++;
        InsertQueue(Q, ppt[p].color, p);
        while (!EmptyQueue(Q)) {
          p = RemoveQueue(Q);
          u.x = p % img->ncols;
          u.y = p / img->ncols;
          for (i=1; i < A->n; i++){
            v.x = u.x + A->dx[i];
            v.y = u.y + A->dy[i];
            if (ValidPixel(img, v.x, v.y)) {
              q = v.x + img->tbrow[v.y];
              if (label->val[q] == NIL && ppt[p].color == ppt[q].color) {
                label->val[q] = label->val[p];
                InsertQueue(Q, ppt[q].color, q);
              }
            }                   
          } 
        }
      }
    }
  DestroyQueue(&Q);
  DestroyAdjRel(&A);

  area = AllocULongArray(nlabels);
  for (i=0;i<nlabels;i++)
    area[i] = 0;

  for (p=0;p<npixels;p++)
    area[label->val[p]]++;

  for (p=0;p<npixels;p++)
    if (100 * area[label->val[p]] < MIN_AREA * npixels)
      ppt[p].frequency = LOW;
    else
      ppt[p].frequency = HIGH;
  DestroyImage(&label);
  free(area);
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
  ComputeFrequencyProperty(cimg->C[0], p, n);
  
  free(color);
  return(p);
}

VisualFeature *ComputeHistograms(Property *p, Image *mask, 
                                 int npixels, int *npoints)
{
  VisualFeature *vf=NULL;
  ulong i;
  
  vf = CreateVisualFeature(SIZE);
  for(i=0; i<SIZE; i++){
    vf->lowH[i] = 0;
    vf->highH[i] = 0;
  }
  
  *npoints = 0;
  for(i=0; i<npixels; i++) {
//    if (mask->val[i]) {
      (*npoints)++;
      if(p[i].frequency==LOW) 
        vf->lowH[p[i].color]++;
      else 
        vf->highH[p[i].color]++;
//    }
  }
  return(vf);
}

CompressedVisualFeature *CompressHistograms(VisualFeature *vf, int npixels)
{
  CompressedVisualFeature *cvf=NULL;
  
  cvf = CreateCompressedVisualFeature(SIZE);
  CompressHistogram(cvf->lowH, vf->lowH, npixels, SIZE);
  CompressHistogram(cvf->highH, vf->highH, npixels, SIZE);
  
  return(cvf);
}

CompressedVisualFeature *ReadCompressedVisualFeatures(char *filename)
{
  CompressedVisualFeature *cvf=NULL;
  FILE *fp;
  int i, n;
  uchar l, h;

  fp = fopen(filename,"r");
  if (fp == NULL){
    fprintf(stderr,"Cannot open %s\n",filename);
    exit(-1);
  }
  fscanf(fp,"%d\n",&n);
  cvf = CreateCompressedVisualFeature(n);
  for (i=0; i<n; i++) {
    fscanf(fp,"%c%c",&l,&h);
    cvf->lowH[i] = l;
    cvf->highH[i] = h;
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
  
  fprintf(fp,"%d\n",SIZE);
  for(i=0;i<SIZE;i++)
    fprintf(fp, "%c%c", cvf->lowH[i], cvf->highH[i]);
  
  fclose(fp);
}

CompressedVisualFeature *ExtractCompressedVisualFeatures(CImage *cimg, 
                                                          Image *mask)
{
  CompressedVisualFeature *cvf=NULL;
  VisualFeature *vf;
  Property *p;
  int npixels;
  int npoints;
  
  npixels = cimg->C[0]->nrows * cimg->C[0]->ncols;
  
  p = ComputePixelsProperties(cimg);
  vf = ComputeHistograms(p, mask, npixels, &npoints);
  cvf = CompressHistograms(vf, npoints);
  
  free(p);
  DestroyVisualFeature(&vf);
  return(cvf);
}

Histogram *CCV(CImage *cimg, Image *mask)
{
  Histogram *histogram = NULL;
  CompressedVisualFeature *cvf;
  int i;
  
  cvf = ExtractCompressedVisualFeatures(cimg, mask);

  histogram = CreateHistogram(2*SIZE);  
  for (i=0; i<SIZE; i++){
    histogram->v[i] = cvf->lowH[i];
    histogram->v[i+SIZE] = cvf->highH[i];
  }
  DestroyCompressedVisualFeature(&cvf);

  return(histogram);
}

