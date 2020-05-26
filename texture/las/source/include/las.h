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
#ifndef _LAS_H_
#define _LAS_H_

#include "image.h"
#include "cimage.h"
#include "common.h"

#define QTD_BINS 4  //qtd de bins para cada gi

typedef struct _imageHSV {
  float H,S,V;
} ImageHSV;

/*Funcoes auxiliares*/
/*
ImageHSV **CreateImageHSV(int ncols, int nrows);
void DestroyImageHSV(ImageHSV **img, int nrows);
*/
void RGB2HSV_las(CImage *RGB, ImageHSV **HSV);
void CalculaGradientes(Image *img, Image *g1, Image *g2, Image *g3, Image *g4);
float *HistogramaGradiente(Image *g1, Image *g2, Image *g3, Image *g4);
void NormalizaHistograma(float *hist, int hist_size, int qtd_pixels);

double *ReadFileDouble(char *filename, int size);
float *ReadFileFloat(char *filename, int size);
void WriteFileDouble(char *filename, double *vet, int size);
void WriteFileFloat(char *filename, float *vet, int size);

double L2DoubleDistance(double *v1, double *v2, int size);
double L1FloatDistance(float *v1, float *v2, int size);

#endif
