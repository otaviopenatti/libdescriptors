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
#include "csd.h"

#include "common.h"
#include "image.h"

#include "color.h"

#define SIZE 184

// Convert an image from RGB to HMMD color space
CImage *CImageRGBtoHMMD(CImage *cimg)
{
  CImage *ncimg=NULL;
  int p,n,i;


  ncimg = CreateCImage(cimg->C[0]->ncols,cimg->C[0]->nrows);
  n    = ncimg->C[0]->ncols*ncimg->C[0]->nrows;

  for (p=0; p < n; p++){

    i = triplet(cimg->C[0]->val[p],cimg->C[1]->val[p],cimg->C[2]->val[p]);
    i = RGB2HMMD(i);
    ncimg->C[0]->val[p]=t0(i);
    ncimg->C[1]->val[p]=t1(i);
    ncimg->C[2]->val[p]=t2(i);
  }

  return(ncimg);
}

#define GMAX(a,b,c) (((a>=b)&&(a>=c))?a:((b>c)?b:c))
#define GMIN(a,b,c) (((a<=b)&&(a<=c))?a:((b<c)?b:c))

// Convert RGB to HMMD color space (just Hue, Diff and Sum)
int RGB2HMMD(int vi) {
  float r = (float) t0(vi), 
        g = (float) t1(vi), 
        b = (float) t2(vi), max, min, f; 
  float a[3];

  int i; 

  r/=255.0;
  g/=255.0;
  b/=255.0;

  // RGB are each on [0, 1]. Hue, Diff and Sum are returned on [0, 1]. 
  min = GMIN(r, g, b); 
  max = GMAX(r, g, b); 
  if (max == min) {
    a[0]=0.0;
    a[1]=0.0;
    a[2]=max;
  } else {
    f = (r == min) ? g - b : ((g == min) ? b - r : r - g); 
    i = (r == min) ? 3 : ((g == min) ? 5 : 1);
    a[0]=((float)i)-f/(max-min);
    a[1]=max-min;
    a[2]=(max+min)/2;
  }

  // (un)normalize
  a[0]*=255.0;
  a[1]*=255.0;
  a[2]*=255.0;
  
  return(triplet(ROUND(a[0]),ROUND(a[1]),ROUND(a[2])));
}

Histogram *CSD(CImage *cimg)
{
  Histogram *histogram = NULL;
  CompressedVisualFeature *cvf;
  int i;
  CImage *cimg_hmmd;

  // Convert from RGB to HMMD color space
  cimg_hmmd = CImageRGBtoHMMD(cimg);

  cvf = ExtractCompressedVisualFeatures(cimg_hmmd);

  histogram = CreateHistogram(SIZE);  
  for (i=0; i<SIZE; i++)
    histogram->v[i] = cvf->colorH[i];
  DestroyCompressedVisualFeature(&cvf);

  DestroyCImage(&cimg_hmmd);
  return(histogram);
}

uchar ComputeNorm(float value)
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

CompressedVisualFeature *CompressHistograms(VisualFeature *vf, int npixels)
{
  CompressedVisualFeature *cvf=NULL;
  
  cvf = CreateCompressedVisualFeature(SIZE);
  CompressHistogram(cvf->colorH, vf->colorH, npixels, SIZE);
  
  return(cvf);
}

CompressedVisualFeature *ExtractCompressedVisualFeatures(CImage *cimg)
{
  CompressedVisualFeature *cvf=NULL;
  VisualFeature *vf;
  Property *p;
  int npixels;
  int npoints;

  npixels = cimg->C[0]->nrows * cimg->C[0]->ncols;

  p = ComputePixelsProperties(cimg);
  vf = ComputeHistograms(p, cimg->C[0]->nrows, cimg->C[0]->ncols, npixels, &npoints);

  // Normalize
  cvf = CompressHistograms(vf, npoints);

  free(p);
  DestroyVisualFeature(&vf);
  return(cvf);
}

Property *ComputePixelsProperties(CImage *cimg)
{
  Property *p=NULL;
  int *color, i, n;

  n = cimg->C[0]->nrows * cimg->C[0]->ncols;  

  p = AllocPropertyArray(n);

  color = QuantizeColors(cimg);
  for(i=0; i<n; i++) 
    p[i].color=color[i];

  free(color);
  return(p);
}

int *QuantizeColors(CImage *cimg)
{
  ulong i;
  float hue, diff, sum;
  ulong quant_level_hue=0, quant_level_sum=0, start_diff=0;
  int *color, n;

  n = cimg->C[0]->nrows * cimg->C[0]->ncols;  

  color = AllocIntArray(n);

  for(i=0; i<n; i++){

    hue = cimg->C[0]->val[i]/255.0;
    diff = cimg->C[1]->val[i];
    sum = cimg->C[2]->val[i]/255.0;

    // Check diff intervals [0,6), [6,20), [20, 60), [60, 110) and [110, 255]
    if (diff >=0 && diff < 6) {
        quant_level_sum = 8;
        quant_level_hue = 1;
        start_diff = 0;
    } else if (diff >=6 && diff < 20) {
        quant_level_sum = 4;
        quant_level_hue = 8;
        start_diff = 8;
    } else if (diff >=20 && diff < 60) {
        quant_level_sum = 4;
        quant_level_hue = 12;
        start_diff = 40;
    } else if (diff >=60 && diff < 110) {
        quant_level_sum = 4;
        quant_level_hue = 12;
        start_diff = 88;
    } else if (diff >=110 && diff <= 255) {
        quant_level_sum = 2;
        quant_level_hue = 24;
        start_diff = 136;
    };

    // Linearize
    color[i] = (start_diff + hue * (quant_level_hue - 1) * quant_level_sum + sum * (quant_level_sum - 1));
  }
  return(color);
}

VisualFeature *ComputeHistograms(Property *p, int nrows, int ncols, int npixels, int *npoints)
{
    VisualFeature *vf=NULL, *vf_tmp=NULL;
    ulong i, j, c;

    // Coordinates of the structuring element
    ulong x, y;

    int p2;

    // Sub-sampling factor
    int k;

    // Spatial extent of the structuring element
    int e;

    // Calculate sub-sampling factor and spatial extent of the structuring element
    //p2 = ROUND(0.5 * log((npixels) / log(2)) - 8);
    p2 = ROUND(0.5*(log(npixels) / log(2)) - 8);
    //printf("p2 = %d\n", p2);
    if (p2<0) {
        p2 = 0;
    }
    k = pow(2, p2);
    e = 8 * k;

    vf = CreateVisualFeature(SIZE);
    for(c=0; c<SIZE; c++){
        vf->colorH[c] = 0;
    }
    vf_tmp = CreateVisualFeature(SIZE);

    *npoints = 0;

    // Scan the image
    for (i = 0; i <= (nrows - e); i = i + k) {

        for (j = 0; j <= (ncols - e); j = j + k) {

            (*npoints)++;

            // Clear temporary histogram for the structuring element
            for(c=0; c<SIZE; c++){
                vf_tmp->colorH[c] = 0;
            }

            // Count colors inside the structuring element
            for (y = i; y < (i + e); y = y + k) {
                for (x = j; x < (j + e); x = x + k) {
                    if (vf_tmp->colorH[p[x + ncols * y].color] == 0) {
                        vf_tmp->colorH[p[x + ncols * y].color] = 1;
                    }
                }
            }
            // Accumulate the histogram
            for(c=0; c<SIZE; c++){
                vf->colorH[c] = vf->colorH[c] + vf_tmp->colorH[c];
            }
        }

    }

    DestroyVisualFeature(&vf_tmp);
    return(vf);
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

Property *AllocPropertyArray(int n)
{
  Property *v=NULL;
  v = (Property *) calloc(n,sizeof(Property));
  if (v==NULL)
    Error(MSG1,"AllocPropertyArray");
  return(v);
}

// Calculate L1 distance
double CSDDistance(Histogram *h1, Histogram *h2) { 
  ulong i;
  ulong sum = 0;
  ulong z = 0;
  
  for (i = 0; i < h1->n ; i++){
    z = fabs(h1->v[i] - h2->v[i]); 
    sum = sum + z;
  }
  return ((double) sum);
}

