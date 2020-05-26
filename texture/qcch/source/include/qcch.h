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
#ifndef _QCCH_H_
#define _QCCH_H_

#include "image.h"
#include "cimage.h"
#include "common.h"

#define RAIO     1
#define QTD_VIZ  (1+(4*RAIO)+(4*RAIO*RAIO))  //this value depends on the radio=a (tested for values up to 4)
#define QTD_BINS 40  //number of bins t(i,j) in the non-uniformly quantized space

Image *RGB2Cinza(CImage *image);
long int SomaJanela(int centroI, int centroJ, int raio, Image *img);

void Rate_Change_Gray(Image *image, Image *t, int r);
void Change_Histogram(Image *t, double h[], int r);
double *QCCH(CImage *cimg, int raio);

double Dist_Measure(double Ha[],double Hb[]);

void ReadFile(char *filename, double h[]);
double *ReadFileBin(char *filename, int size);

void WriteStream(double *h,FILE *fp);
void WriteFile(double h[],char *filename);
void WriteFileBin(double *h, char *filename, int size);

#endif
