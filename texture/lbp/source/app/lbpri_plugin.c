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
    Image *img=NULL;
    Histogram *lbp=NULL;
    int i;

    cimg = ReadCImage(img_path);
    img = CreateImage(cimg->C[0]->ncols, cimg->C[0]->nrows);

    //translate to HSV space
    for (i=0; i<(img->ncols*img->nrows); i++) {
        img->val[i] = (cimg->C[0]->val[i])+(cimg->C[1]->val[i])+(cimg->C[2]->val[i])/3;
    }

    lbp = LBPri(img);

    WriteFileHistogram(lbp,fv_path);
    DestroyHistogram(&lbp);
    DestroyCImage(&cimg);
    DestroyImage(&img);

}


double Distance(char *fv1_path, char *fv2_path)
{
  
  Histogram *lbp1=NULL;
  Histogram *lbp2=NULL;
  double distance;

  lbp1 = ReadFileHistogram(fv1_path);
  lbp2 = ReadFileHistogram(fv2_path);

  distance=(double)L1Distance(lbp1, lbp2);

  DestroyHistogram(&lbp1);
  DestroyHistogram(&lbp2);

  return (distance);
}
