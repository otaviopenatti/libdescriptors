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
#ifndef _LUCOLOR2_H_
#define _LUCOLOR2_H_

#include "cimage.h"
#include "image.h"
#include "common.h"

#define QTD_BLOCOS 100

typedef struct vetor {
    float *r;
    float *g;
    float *b;
    int *tam;
}Vetor;

Vetor *CreateVetor(long int blocos);
void DestroyVetor(Vetor **v);
Vetor *ReadFileVetor(char *filename, double *media, double *desvio);
Vetor *ReadFileVetorBin(char *filename, double *media, double *desvio);
void WriteFileVetor(Vetor *h,char *filename,double *media, double *desvio);
void WriteStreamVetor(Vetor *h,FILE *fp, double *media, double *desvio);
void WriteFileVetorBin(Vetor *h,char *filename,double *media, double *desvio);
void WriteStreamVetorBin(Vetor *h,FILE *fp, double *media, double *desvio);

void MediaImage(CImage *image, double *media, long int tam);
void DesvioImage(CImage *image, double *media,double *desvio, long int tam);
Vetor *LuColor2(CImage *image, long int tam, double *media, double *desvio);
void Bitmap2(double *media,Vetor *mbloco,Vetor *bitmap, long int blocos);
void MediaBlocos2(CImage *imagem,Vetor *mbloco,long int tam,long int n, int qtde);

double Hamming(Vetor *Ta,Vetor *Tb, double *ma, double *mb, double *da, double *db);

#endif
