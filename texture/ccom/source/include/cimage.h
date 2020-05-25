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
#ifndef _CIMAGE_H_
#define _CIMAGE_H_

#include "image.h"

typedef struct cimage {
  Image *C[3];
} CImage;


CImage *CreateCImage(int ncols, int nrows);
void    DestroyCImage(CImage **cimg);
CImage *ReadCImage(char *filename);
void    WriteCImage(CImage *cimg, char *filename);
CImage *CopyCImage(CImage *cimg);
CImage *Convert2CImage(Image *img);
CImage *CLinearStretch(CImage *cimg, int f1, int f2, int g1, int g2);
CImage *CROI(CImage *cimg, int xl, int yl, int xr, int yr);
CImage *CScale(CImage *cimg, float Sx, float Sy);
CImage *COverlayAlphaImage (CImage *cimg, Image *alpha);
Image  *GetBand(CImage *cimg, char band);
CImage *ColorizeImage(Image *img, float R, float G, float B);
CImage *BlendImages(CImage *cimg1, CImage *cimg2, float alpha1, float alpha2);
CImage *CImageRGBtoYCbCr(CImage *cimg);
CImage *CImageYCbCrtoRGB(CImage *cimg);
CImage *CImageRGBtoHSV(CImage *cimg);
CImage *CImageHSVtoRGB(CImage *cimg);
CImage *CImageRGBtoXYZ(CImage *cimg);
CImage *CImageXYZtoRGB(CImage *cimg);
CImage *CImageXYZtoLAB(CImage *cimg);
CImage *CImageLABtoXYZ(CImage *cimg);
void    DrawCPoint(CImage *cimg, int x, int y, float raio, int R, int G, int B);
CImage *DrawCBorder(Image *img, Image *label);


#endif
