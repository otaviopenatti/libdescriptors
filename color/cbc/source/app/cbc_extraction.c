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

int main(int argc, char** argv)
{
  CImage *cimg=NULL;
  Image *mask=NULL;
  Ap_FeatureVector1D *cbc=NULL;
  int i, n;

  if (argc != 3) {
    fprintf(stderr,"Usage: cbc_extraction <img_path> <fv_path>\n");
	fprintf(stderr,"\timg_path: path to the image file in PPM (P6) format\n");
	fprintf(stderr,"\tfv_path:  path to the file that will store the feature vector\n");
	fprintf(stderr,"Remark:\n\tThis program extracts the feature vector from a single image.\n");
    exit(-1);
  }  

  cimg = ReadCImage(argv[1]);
  mask = CreateImage(cimg->C[0]->ncols, cimg->C[0]->nrows);

  cbc = CBC(cimg, mask, &n);

  WriteAp_FeatureVector1D(cbc, n, argv[2]);

  for (i=0;i<n;i++)
    DestroyFeatureVector1D(&cbc[i]);
  free(cbc);

  DestroyCImage(&cimg);
  DestroyImage(&mask);

  return(0);
}
