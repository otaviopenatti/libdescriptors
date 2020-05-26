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
#include "libdescriptors.h"

void Extraction(char *img_path, char *fv_path)
{
  CImage *cimg=NULL;
  Image *mask=NULL;
  Ap_FeatureVector1D *cbc=NULL;
  int i, n;

  cimg = ReadCImage(img_path);
  mask = CreateImage(cimg->C[0]->ncols, cimg->C[0]->nrows);

  cbc = CBC(cimg, mask, &n);

  WriteAp_FeatureVector1D(cbc, n, fv_path);

  for (i=0;i<n;i++)
    DestroyFeatureVector1D(&cbc[i]);
  free(cbc);

  DestroyCImage(&cimg);
  DestroyImage(&mask);

}

double Distance(void *fv1_path, void *fv2_path)
{
  Ap_FeatureVector1D *cbc1=NULL;
  Ap_FeatureVector1D *cbc2=NULL;
  FeatureVector1D *wcbc1=NULL;
  FeatureVector1D *wcbc2=NULL;
  double distance;
  int n1, n2;
  int i;

  cbc1 = ReadAp_FeatureVector1D(fv1_path, &n1);
  // cbc1 = (Ap_FeatureVector1D *) fv1; 
  cbc2 = ReadAp_FeatureVector1D(fv2_path, &n2);
  // cbc2 = (Ap_FeatureVector1D *) fv2; 

  wcbc1 = CreateFeatureVector1D(n1);
  for (i=0; i<n1; i++)
    wcbc1->X[i] = cbc1[i]->X[5];

  wcbc2 = CreateFeatureVector1D(n2);
  for (i=0; i<n2; i++)
    wcbc2->X[i] = cbc2[i]->X[5];

  distance=IRM(cbc1, wcbc1, cbc2, wcbc2, CBCRegionDistance);

  for (i=0; i<n1; i++)
    DestroyFeatureVector1D(&cbc1[i]);
  free(cbc1);

  for (i=0; i<n2; i++)
    DestroyFeatureVector1D(&cbc2[i]);
  free(cbc2);

  DestroyFeatureVector1D(&wcbc1);
  DestroyFeatureVector1D(&wcbc2);

  return distance;
}
