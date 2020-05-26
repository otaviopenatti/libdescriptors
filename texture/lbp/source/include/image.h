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
#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "common.h"

typedef struct _pixel {
  int x,y;
} Pixel;

typedef struct _image {
  int *val;
  int ncols,nrows;
  int *tbrow;
} Image;

Image  *CreateImage(int ncols,int nrows);
void    DestroyImage(Image **img);
Image  *ReadImage(char *filename);
void    WriteImage(Image *img, char *filename);
Image  *ConvertToNbits(Image *img, int N);
int     MinimumValue(Image *img);
int     MaximumValue(Image *img);
void    SetImage(Image *img, int value);
bool    ValidPixel(Image *img, int x, int y);
Image  *CopyImage(Image *img);
Image  *AddFrame(Image *img, int sz, int value);
Image  *RemFrame(Image *fimg, int sz);
Image  *ROI(Image *img, int xl, int yl, int xr, int yr);
Image  *MBB(Image *img);
Image  *MakeFrame(int ncols, int nrows, int value);
void    PaintCircle(Image *img, int center, float radius, int value);
void    DrawPath(Image *img,Image *pred, int dst,int value);
Image  *InputSeeds(int ncols, int nrows, char *seeds);
Image  *Rotate(Image *img, float theta); 
Image  *Scale(Image *img, float Sx, float Sy); 
Image  *ImageMagnitude(Image *imgx, Image *imgy);
Image  *MakeImage(char *data);
Image  *AddZeros(Image *img, int ncols, int nrows);
Image  *CreateSine(int ncols, int nrows, float wx, float wy, float phix, float phiy);
Image  *CreateCos(int ncols, int nrows, float wx, float wy, float phix, float phiy);
Image  *CreateSquare(int xo, int yo, int xf, int yf, int ncols, int nrows, int val);
Image  *CreateIntArray(char *data);
Image  *OverlayAlphaImage (Image *img, Image *alpha);
int    DistEucl(Image *img, int p, int q);

#endif





