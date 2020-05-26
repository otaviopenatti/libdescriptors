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
#ifndef _COLORCOOCURRENCE_H_
#define _COLORCOOCURRENCE_H_

#include "common.h"
#include "image.h"
#include "cimage.h"

#define COLORDIM 6 /*dimensions to quantize the RGB space*/

#define AMIN 0.002 
#define AMAX 0.200

#define DISTMAX 1 /*maximum distance between pixels*/

#define MAX_VAL_DIST 99999 /*maximum value obtained by the distance function*/
            /*maximum distance is 1; the value above is used to differentiate images with no intersection from images with empty vectors */


typedef struct _cco
{
  int i;/*index of the element with non-zero value*/
  float w; /*value of the element*/
} CCO;

typedef struct _ccofv
{
  int size;
  CCO *FV;
} CCOFV;

Image *QuantColors(CImage *cimg, int color_dim);
float ***MakeW(int size, int max_dist );
float ***CreateW(Image *Qimg, int color_dim, int max_dist);
void DestroyW(float ****W, int color_dim, int max_dist);
int cityBlock(int i1, int j1, int i2, int j2);
CCOFV*  makeCCOFV(float ***W, int color_dim, int max_dist, float Amin, float Amax);
void DestroyCCOFV(CCOFV **fv);

#endif
