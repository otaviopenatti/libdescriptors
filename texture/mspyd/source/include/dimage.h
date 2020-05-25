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
#ifndef _DIMAGE_H_
#define _DIMAGE_H_

#include "common.h"
#include "image.h"

typedef struct _DImage{
  double *val;
  int ncols,nrows;
  int *tbrow;
} DImage;


DImage *CreateDImage(int ncols, int nrows);
void    DestroyDImage(DImage **dimg);

DImage  *ReadDImage(char *filename);
void    WriteDImage(DImage *dimg, char *filename);
double  MinimumDImageValue(DImage *dimg);
double  MaximumDImageValue(DImage *dimg);
void    SetDImage(DImage *dimg, double value);
bool    ValidDImagePixel(DImage *dimg, int x, int y);
DImage  *CopyDImage(DImage *dimg);
Image   *ConvertDImage2Image(DImage *dimg);

DImage 	*ConvertImage2DImage(Image* img);
DImage  *DScale(DImage* img, float Sx, float Sy);

/** Increases the size of DImage until it becomes power-of-2-sided.
 *  The resulting image IS NOT squared.
 **/
DImage* DImagePower2(DImage* img);

/** Shifts image like FFTShift in spectrum.h.
 *  Used mostly in gabor.h.
 **/
DImage* DShift(DImage* img);

#endif
