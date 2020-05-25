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
#ifndef _SASI_H_
#define _SASI_H_

#include "image.h"
#include "featurevector.h"
#include "cimage.h"
#include "common.h"

typedef struct _imageHSV {
  float H,S,V;
} ImageHSV;


FeatureVector1D *SASI(Image *img, int num, int *sizes);

void RGB2HSV_sasi(CImage *RGB, ImageHSV **HSV);
FeatureVector1D *ReadFileFeatureVector1DBin(char *filename);

#endif
