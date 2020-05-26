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
#ifndef _FEATUREVECTOR_H_
#define _FEATUREVECTOR_H_

#include "common.h"

/*data structure used to compute distances */
typedef struct _FeatureVector1D {
  double *X;
  int n; 
} FeatureVector1D;

typedef struct _FeatureVector2D {
  double *X;
  double *Y;
  int n; 
} FeatureVector2D;

typedef FeatureVector1D *Ap_FeatureVector1D;
typedef FeatureVector2D *Ap_FeatureVector2D;

FeatureVector1D     *CreateFeatureVector1D(int n);
void                 DestroyFeatureVector1D(FeatureVector1D **fv);
Ap_FeatureVector1D  *AllocAp_FeatureVector1DArray(int n);
FeatureVector1D     *ReadStreamFeatureVector1D(FILE *fp);
FeatureVector1D     *ReadFileFeatureVector1D(char *filename);
Ap_FeatureVector1D  *ReadAp_FeatureVector1D(char *filename, int *n);
void                 WriteStreamFeatureVector1D(FeatureVector1D *fv,FILE *fp);
void                 WriteFileFeatureVector1D(FeatureVector1D *fv,char *filename);
void                 WriteAp_FeatureVector1D(Ap_FeatureVector1D *ap_fv,int n,char *filename);
FeatureVector1D     *CopyFeatureVector1D(FeatureVector1D *fv);
void                 SortFeatureVector1D(FeatureVector1D *fv, int left, int right, char order);
int                  PartFeatureVector1D(FeatureVector1D *fv, int left, int right, char order);

FeatureVector2D     *CreateFeatureVector2D(int n);
void                 DestroyFeatureVector2D(FeatureVector2D **fv);
Ap_FeatureVector2D  *AllocAp_FeatureVector2DArray(int n);
FeatureVector2D     *ReadStreamFeatureVector2D(FILE *fp);
FeatureVector2D     *ReadFileFeatureVector2D(char *filename);
Ap_FeatureVector2D  *ReadAp_FeatureVector2D(char *filename,int *n);
void                 WriteStreamFeatureVector2D(FeatureVector2D *fv,FILE *fp);
void                 WriteFileFeatureVector2D(FeatureVector2D *fv,char *filename);
void                 WriteAp_FeatureVector2D(Ap_FeatureVector2D *ap_fv,int n,char *filename);
FeatureVector2D     *CopyFeatureVector2D(FeatureVector2D *fv);
void                 SortFeatureVector2D(FeatureVector2D *fv, int left, int right, char order);
int                  PartFeatureVector2D(FeatureVector2D *fv, int left, int right, char order);
void                 FeatureVector2DInvertXY(FeatureVector2D *fv);

#endif

