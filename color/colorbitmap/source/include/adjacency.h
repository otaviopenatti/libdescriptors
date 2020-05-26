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
#ifndef _ADJACENCY_H_
#define _ADJACENCY_H_

#include "image.h"

typedef struct _adjrel {
  int *dx;
  int *dy;
  int n;
} AdjRel;

typedef struct _adjpxl {
  int *dp;
  int n;
} AdjPxl;


/* All adjacency relations must place the central pixel at first. The
   central pixel must be skipped when visiting the neighbors during
   the IFT. It must be considered in the other operations. */

AdjRel *CreateAdjRel(int n);
void    DestroyAdjRel(AdjRel **A);
AdjRel *RightSide(AdjRel *A);
AdjRel *LeftSide(AdjRel *A);
AdjRel *RightSide2(AdjRel *A, float r);
AdjRel *LeftSide2(AdjRel *A, float r);
AdjRel *Circular(float r);
AdjPxl *AdjPixels(Image *img,AdjRel *A);
void    DestroyAdjPxl(AdjPxl **N);
int     FrameSize(AdjRel *A);
AdjRel *ComplAdj(AdjRel *A1, AdjRel *A2);
AdjRel *Horizontal(int r);
AdjRel *Vertical(int r);
AdjRel *Box(int ncols, int nrows);
AdjRel *ShearedBox(int xsize, int ysize, float Si, float Sj);

#endif
