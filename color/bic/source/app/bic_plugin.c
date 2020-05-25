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
  Histogram *bic=NULL;

  cimg = ReadCImage(img_path);
  mask = CreateImage(cimg->C[0]->ncols, cimg->C[0]->nrows);

  bic = BIC(cimg, mask);

  WriteFileHistogram(bic,fv_path);
  DestroyHistogram(&bic);
  DestroyImage(&mask);
  DestroyCImage(&cimg);

}

double Distance(void *fv1_path, void *fv2_path)
{
  Histogram *bic1=NULL;
  Histogram *bic2=NULL;
  double distance;

  bic1 = ReadFileHistogram(fv1_path);
  bic2 = ReadFileHistogram(fv2_path);

  distance = (double) L1Distance(bic1, bic2);

  DestroyHistogram(&bic1);
  DestroyHistogram(&bic2);

  return distance;
}
