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
#ifndef _CSD_H_
#define _CSD_H_

#include "cimage.h"
#include "histogram.h"

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

CImage *CImageRGBtoHMMD(CImage *cimg);
int RGB2HMMD(int vi);
Histogram *CSD(CImage *cimg);
CompressedVisualFeature *ExtractCompressedVisualFeatures(CImage *cimg);
Property *ComputePixelsProperties(CImage *cimg);
int *QuantizeColors(CImage *cimg);
void CompressHistogram(uchar *ch, ulong *h, ulong max, int size);
CompressedVisualFeature *CompressHistograms(VisualFeature *vf, int npixels);
void DestroyCompressedVisualFeature(CompressedVisualFeature **cvf);
uchar ComputeNorm(float value);
VisualFeature *ComputeHistograms(Property *p, int nrows, int ncols, int npixels, int *npoints);
CompressedVisualFeature *CreateCompressedVisualFeature(int n);
void DestroyVisualFeature(VisualFeature **vf);
VisualFeature *CreateVisualFeature(int n);
Property *AllocPropertyArray(int n);
double CSDDistance(Histogram *h1, Histogram *h2);

#endif
